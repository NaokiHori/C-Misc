#include <stddef.h> // size_t
#include <stdbool.h>
#include <math.h>
#include <float.h> // DBL_EPSILON
#include "config.h"
#include "output.h"
#include "field.h"
#include "grid.h"
#include "acdi.h"

static double compute_sdf (
    const double eps,
    double vof
) {
  vof = fmax(0. + DBL_EPSILON, vof);
  vof = fmin(1. - DBL_EPSILON, vof);
  const double num = 0. + vof;
  const double den = 1. - vof;
  return eps * log(num / den);
}

static int compute_flux (
    const config_t * const config,
    const double gamma,
    const double eps,
    const double * const vof,
    double * const flux
) {
  const size_t nitems = config->nitems;
  const double dx = compute_dx(config);
  const double velocity = config->velocity;
  for (size_t i = 1; i <= nitems; i++) {
    const double adv = - velocity * (
        + 0.5 * vof[i - 1]
        + 0.5 * vof[i    ]
    );
    const double dif1 = gamma * eps / dx * (
        - vof[i - 1]
        + vof[i    ]
    );
    double dif2 = 0.;
    {
      const double sdf_xm = compute_sdf(eps, vof[i - 1]);
      const double sdf_xp = compute_sdf(eps, vof[i    ]);
      const double sdf = (
          + 0.5 * sdf_xm
          + 0.5 * sdf_xp
      );
      const double normal = sdf_xm < sdf_xp ? + 1. : sdf_xp < sdf_xm ? - 1. : 0.;
      dif2 = - gamma * 0.25 * (1. - pow(tanh(0.5 / eps * sdf), 2.)) * normal;
    }
    flux[i] = adv + dif1 + dif2;
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
    vof[i] += dt / dx * (
        - flux[i    ]
        + flux[i + 1]
    );
  }
  exchange_halo(config, vof);
  return 0;
}

int acdi (
    const config_t * const config,
    const double output_freq
) {
  output_config_t output_config = {
    .freq = output_freq,
    .next = output_freq,
    .cntr = 0,
  };
  const double gamma = 1.;
  const double eps = 0.75 * compute_dx(config);
  double * const x = allocate_field(config);
  double * const vof = allocate_field(config);
  double * const flux = allocate_field(config);
  init_grid(config, x);
  init_vof(config, x, vof);
  for (double time = 0.; time < config->time_max; ) {
    const double dt = decide_dt(config);
    compute_flux(config, gamma, eps, vof, flux);
    update_vof(config, dt, flux, vof);
    time += dt;
    output(config, &output_config, __func__, time, x, vof);
  }
  deallocate_field(x);
  deallocate_field(vof);
  deallocate_field(flux);
  return 0;
}
