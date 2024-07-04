#include "config.h"

double compute_dx (
    const config_t * const config
) {
  return config->length / config->nitems;
}

double decide_dt (
    const config_t * const config
) {
  // small-enough value to mitigate time-marching error
  const double cfl = 0.1;
  const double abs_vel = 0. < config->velocity ? config->velocity : - config->velocity;
  return cfl * compute_dx(config) / abs_vel;
}

