#include <stddef.h> // size_t
#include "config.h"
#include "output.h"
#include "field.h"
#include "grid.h"
#include "upwind.h"

static int compute_flux (
    const config_t * const config,
    const double * const vof,
    double * const flux
) {
  const size_t nitems = config->nitems;
  const double velocity = config->velocity;
  for (size_t i = 1; i <= nitems; i++) {
    if (0. < velocity) {
      flux[i] = velocity * vof[i - 1];
    } else {
      flux[i] = velocity * vof[i    ];
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

int upwind (
    const config_t * const config,
    const double output_freq
) {
  output_config_t output_config = {
    .freq = output_freq,
    .next = output_freq,
    .cntr = 0,
  };
  double * const x = allocate_field(config);
  double * const vof = allocate_field(config);
  double * const flux = allocate_field(config);
  init_grid(config, x);
  init_vof(config, x, vof);
  for (double time = 0.; time < config->time_max; ) {
    const double dt = decide_dt(config);
    compute_flux(config, vof, flux);
    update_vof(config, dt, flux, vof);
    time += dt;
    output(config, &output_config, __func__, time, x, vof);
  }
  deallocate_field(x);
  deallocate_field(vof);
  deallocate_field(flux);
  return 0;
}
