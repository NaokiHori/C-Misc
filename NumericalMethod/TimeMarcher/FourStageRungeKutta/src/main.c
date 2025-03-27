#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include "type.h"
#include "integrator.h"

#define NDIMS 3

typedef struct {
  // Rayleigh number
  real_t rho;
  // Prandtl number
  real_t sigma;
  real_t beta;
} param_t;

static real_t random_gen_range(
    const real_t min,
    const real_t max
) {
  return (max - min) * rand() / RAND_MAX + min;
}

static int init_positions(
    const size_t nitems,
    real_t * const positions
) {
  for (size_t n = 0; n < nitems; n++) {
    real_t * const position = positions + NDIMS * n;
    position[0] = random_gen_range(-1., +1.);
    position[1] = random_gen_range(-1., +1.);
    position[2] = random_gen_range(-1., +1.);
  }
  return 0;
}

static int lorenz(
    const real_t * const position,
    real_t * const increment
) {
  const param_t param = {
    .rho = 28.,
    .sigma = 10.,
    .beta = 8. / 3.,
  };
  const real_t x = position[0];
  const real_t y = position[1];
  const real_t z = position[2];
  increment[0] = param.sigma * (y - x);
  increment[1] = x * (param.rho - z) - y;
  increment[2] = x * y - param.beta * z;
  return 0;
}

static int output(
    const real_t time,
    const size_t nitems,
    real_t * const positions
) {
  printf("% .7e ", time);
  for (size_t n = 0; n < nitems * NDIMS; n++) {
    printf("% .7e ", positions[n]);
  }
  puts("");
  return 0;
}

int main(
    void
) {
  srand(1 << 10);
  const size_t nitems = 2;
  integrator_t integrator = {0};
  if (0 != integrator_initialize(NDIMS, lorenz, &integrator)) {
    return 1;
  }
  real_t * const positions = malloc(nitems * NDIMS * sizeof(real_t));
  init_positions(nitems, positions);
  const real_t time_max = 5.e+1;
  const real_t dt = 1.e-2;
  for (real_t time = 0.; time < time_max; time += dt) {
    for (size_t n = 0; n < nitems; n++) {
      real_t * const position = positions + NDIMS * n;
      integrator_update(&integrator, dt, position);
      output(time, nitems, positions);
    }
  }
  free(positions);
  if (0 != integrator_finalize(&integrator)) {
    return 1;
  }
  return 0;
}
