#include <stdlib.h>
#include "integrator.h"

int integrator_initialize(
    const size_t ndims,
    int (* kernel)(
      const real_t * const x,
      real_t * const dx
    ),
    integrator_t * const integrator
) {
  integrator->ndims = ndims;
  integrator->buf_x = malloc(ndims * sizeof(real_t));
  for (size_t rk_step = 0; rk_step < RK_ORDER; rk_step += 1) {
    integrator->buf_dx[rk_step] = malloc(ndims * sizeof(real_t));
  }
  integrator->kernel = kernel;
  return 0;
}

int integrator_update(
    integrator_t * const integrator,
    const float dt,
    real_t * const position
) {
  const size_t ndims = integrator->ndims;
  int (* const kernel)(
    const real_t * const x,
    real_t * const dx
  ) = integrator->kernel;
  real_t * const buf_x = integrator->buf_x;
  real_t ** const buf_dx = integrator->buf_dx;
  for (size_t dim = 0; dim < ndims; dim += 1) {
    buf_x[dim] = position[dim];
  }
  kernel(buf_x, buf_dx[0]);
  for (size_t dim = 0; dim < ndims; dim += 1) {
    buf_x[dim] = position[dim] + 0.5 * dt * buf_dx[0][dim];
  }
  kernel(buf_x, buf_dx[1]);
  for (size_t dim = 0; dim < ndims; dim += 1) {
    buf_x[dim] = position[dim] + 0.5 * dt * buf_dx[1][dim];
  }
  kernel(buf_x, buf_dx[2]);
  for (size_t dim = 0; dim < ndims; dim += 1) {
    buf_x[dim] = position[dim] + 1.0 * dt * buf_dx[2][dim];
  }
  kernel(buf_x, buf_dx[3]);
  for (size_t dim = 0; dim < ndims; dim += 1) {
    position[dim] += dt / 6. * (
        + 1. * buf_dx[0][dim]
        + 2. * buf_dx[1][dim]
        + 2. * buf_dx[2][dim]
        + 1. * buf_dx[3][dim]
    );
  }
  return 0;
}

int integrator_finalize(
    integrator_t * const integrator
) {
  free(integrator->buf_x);
  for (size_t rk_step = 0; rk_step < RK_ORDER; rk_step += 1) {
    free(integrator->buf_dx[rk_step]);
  }
  return 0;
}

#if defined(TEST_INTEGRATOR)

#include <stdio.h>
#include <math.h>

static int kernel(
    const real_t * const x,
    real_t * const dx
) {
  // y' = y ( 2 - y )
  *dx = *x * (2. - *x);
  return 0;
}

int main(
    void
) {
  const size_t ndims = 1;
  integrator_t integrator = {0};
  if (0 != integrator_initialize(ndims, kernel, &integrator)) {
    return 1;
  }
  const real_t c3 = log(1. / 3.);
  const real_t time_max = 1.e+0;
  for (real_t dt = 5.e-1; 1.e-4 < dt; dt /= 2.) {
    real_t time = 0.;
    real_t x = 3.;
    for (time = 0.; time < time_max; time += dt) {
      integrator_update(&integrator, dt, &x);
    }
    const real_t answer = - 2. / (exp(c3 - 2. * time) - 1.);
    printf("% .7e % .7e\n", dt, fabs(x - answer));
  }
  if (0 != integrator_finalize(&integrator)) {
    return 1;
  }
  return 0;
}

#endif // TEST_INTEGRATOR
