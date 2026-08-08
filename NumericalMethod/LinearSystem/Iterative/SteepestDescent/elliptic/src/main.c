#include <math.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

#define NDIMS 2

static const double PI = 3.14159265358979324;

static const double lengths[NDIMS] = {
  1.,
  1.,
};

static double compute_laplacian(
    const size_t (* const nitems)[NDIMS],
    const double * const values,
    const size_t i,
    const size_t j
) {
  const size_t nx = (*nitems)[0];
  const size_t ny = (*nitems)[1];
  const double dx = lengths[0] / nx;
  const double dy = lengths[1] / ny;
  const double local_values_x[3] = {
    0 == i ? - values[j * nx + i] : values[j * nx + i - 1],
    values[j * nx + i],
    nx - 1 == i ? - values[j * nx + i] : values[j * nx + i + 1],
  };
  const double local_values_y[3] = {
    0 == j ? - values[j * nx + i] : values[(j - 1) * nx + i],
    values[j * nx + i],
    ny - 1 == j ? - values[j * nx + i] : values[(j + 1) * nx + i],
  };
  return
    - 1. * local_values_x[0] / dx / dx
    + 2. * local_values_x[1] / dx / dx
    - 1. * local_values_x[2] / dx / dx
    - 1. * local_values_y[0] / dy / dy
    + 2. * local_values_y[1] / dy / dy
    - 1. * local_values_y[2] / dy / dy;
}

static int solve(
    const size_t (* const nitems)[NDIMS],
    const double * const b,
    double * const x
) {
  const size_t nx = (*nitems)[0];
  const size_t ny = (*nitems)[1];
  const double tolerance = 1e-8;
  double * const r = malloc(nx * ny * sizeof(double));
  for (size_t n = 0; n < nx * ny; n++) {
    x[n] = 0.;
  }
  for (size_t step = 0; ; step += 1) {
    // compute residual vector
    // r = a * x - b
    for (size_t n = 0; n < nx * ny; n++) {
      r[n] = - b[n];
    }
    for (size_t n = 0; n < nx * ny; n++) {
      const size_t i = n % nx;
      const size_t j = n / nx;
      r[n] += compute_laplacian(nitems, x, i, j);
    }
    // check convergence
    double r_dot_r = 0.;
    for (size_t n = 0; n < nx * ny; n++) {
      r_dot_r += r[n] * r[n];
    }
    printf("step %zu error % .15e\n", step, r_dot_r);
    if (r_dot_r < tolerance) {
      break;
    }
    double r_dot_a_dot_r = 0.;
    for (size_t n = 0; n < nx * ny; n++) {
      const size_t i = n % nx;
      const size_t j = n / nx;
      r_dot_a_dot_r += compute_laplacian(nitems, r, i, j) * r[n];
    }
    const double eta = - r_dot_r / r_dot_a_dot_r;
    // update x
    // x = x + eta * r
    for (size_t n = 0; n < nx * ny; n++) {
      x[n] += eta * r[n];
    }
  }
  free(r);
  return 0;
}

int main(
    void
) {
  const size_t nitems[NDIMS] = {32, 32};
  double * const b = malloc(nitems[0] * nitems[1] * sizeof(double));
  double * const x = malloc(nitems[0] * nitems[1] * sizeof(double));
  for (size_t j = 0; j < nitems[1]; j++) {
    const double dy = lengths[1] / nitems[1];
    const double y = 0.5 * (2 * j + 1) * dy;
    for (size_t i = 0; i < nitems[0]; i++) {
      const double dx = lengths[0] / nitems[0];
      const double x = 0.5 * (2 * i + 1) * dx;
      b[j * nitems[0] + i] =
        - 2. * PI * PI * sin(1. * PI * x) * sin(1. * PI * y)
        - 17. * PI * PI * sin(3. * PI * x) * sin(5. * PI * y);
    }
  }
  if (0 != solve(&nitems, b, x)) {
    return 1;
  }
  free(b);
  free(x);
  return 0;
}
