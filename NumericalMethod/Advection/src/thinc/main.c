#include <stddef.h> // size_t
#include <stdbool.h>
#include <math.h>
#include <float.h> // DBL_EPSILON
#include "config.h"
#include "output.h"
#include "field.h"
#include "grid.h"
#include "thinc.h"

static double indicator_function (
    const double beta,
    const double a,
    const double b,
    const double x
) {
  const double surface_function = a * x + b;
  return 0.5 * (1. + tanh(beta * surface_function));
}

static int compute_metrics (
    const config_t * const config,
    const double beta,
    const double vof_min,
    const double * const vof,
    double * const a,
    double * const b
) {
  const size_t nitems = config->nitems;
  for (size_t i = 1; i <= nitems; i++) {
    if (vof[i] < vof_min || 1. - vof_min < vof[i]) {
      // almost single-phase, no reconstruction
      continue;
    }
    // normal vector, by means of Youngs approach
    const double n_xm = vof[i - 1] < vof[i    ] ? + 1. : - 1.;
    const double n_xp = vof[i    ] < vof[i + 1] ? + 1. : - 1.;
    const double n_xc = 0.5 * n_xm + 0.5 * n_xp;
    a[i] = n_xc / fmax(fabs(n_xc), DBL_EPSILON);
    // signed-distance function
    b[i] = - 0.5 / beta * log(1. / vof[i] - 1.);
  }
  exchange_halo(config, a);
  exchange_halo(config, b);
  return 0;
}

static int compute_flux (
    const config_t * const config,
    const double beta,
    const double vof_min,
    const double * const vof,
    const double * const a,
    const double * const b,
    double * const flux
) {
  const size_t nitems = config->nitems;
  const double velocity = config->velocity;
  for (size_t i = 1; i <= nitems; i++) {
    // use upwind information
    const bool is_negative_velocity = velocity < 0.;
    const size_t ii = is_negative_velocity ?     i : i - 1;
    const double  x = is_negative_velocity ? - 0.5 : + 0.5;
    if (vof[ii] < vof_min || 1. - vof_min < vof[ii]) {
      flux[i] = vof[ii];
    } else {
      flux[i] = velocity * indicator_function(beta, a[ii], b[ii], x);
    }
  }
  exchange_halo(config, flux);
  return 0;
}

static int update_vof (
    const config_t * const config,
    const double dt,
    const double * const flux,
    double * const vof
) {
  const size_t nitems = config->nitems;
  const double dx = compute_dx(config);
  for (size_t i = 1; i <= nitems; i++) {
    vof[i] += dt * - 1. / dx * (
        - flux[i    ]
        + flux[i + 1]
    );
  }
  exchange_halo(config, vof);
  return 0;
}

int thinc (
    const config_t * const config,
    const double output_freq
) {
  output_config_t output_config = {
    .freq = output_freq,
    .next = output_freq,
    .cntr = 0,
  };
  const double beta = 2.;
  const double vof_min = 1.e-8;
  double * const x = allocate_field(config);
  double * const vof = allocate_field(config);
  double * const flux = allocate_field(config);
  // surface function: a x + b
  double * const a = allocate_field(config);
  double * const b = allocate_field(config);
  init_grid(config, x);
  init_vof(config, x, vof);
  for (double time = 0.; time < config->time_max; ) {
    const double dt = decide_dt(config);
    compute_metrics(config, beta, vof_min, vof, a, b);
    compute_flux(config, beta, vof_min, vof, a, b, flux);
    update_vof(config, dt, flux, vof);
    time += dt;
    output(config, &output_config, __func__, time, x, vof);
  }
  deallocate_field(x);
  deallocate_field(vof);
  deallocate_field(flux);
  deallocate_field(a);
  deallocate_field(b);
  return 0;
}
