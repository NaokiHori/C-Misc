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

static double compute_inner_product(
    const size_t nitems,
    const double * const a,
    const double * const b
) {
  double inner_product = 0.;
  for (size_t i = 0; i < nitems; i++) {
    inner_product += a[i] * b[i];
  }
  return inner_product;
}

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
  double * const p = malloc(nx * ny * sizeof(double));
  double * const a_dot_p = malloc(nx * ny * sizeof(double));
  for (size_t n = 0; n < nx * ny; n++) {
    x[n] = 0.;
  }
  // r = a * x - b
  for (size_t n = 0; n < nx * ny; n++) {
    const size_t i = n % nx;
    const size_t j = n / nx;
    r[n] = compute_laplacian(nitems, x, i, j) - b[n];
    p[n] = r[n];
  }
  double r_dot_r = compute_inner_product(nx * ny, r, r);
  for (size_t step = 0; ; step += 1) {
    // check convergence
    printf("step %zu error % .1e\n", step, r_dot_r);
    if (r_dot_r < tolerance) {
      break;
    }
    // compute eta
    for (size_t n = 0; n < nx * ny; n++) {
      const size_t i = n % nx;
      const size_t j = n / nx;
      a_dot_p[n] = compute_laplacian(nitems, p, i, j);
    }
    const double p_dot_a_dot_p = compute_inner_product(nx * ny, p, a_dot_p);
    const double eta = - r_dot_r / p_dot_a_dot_p;
    // x = x + eta * p
    // r = r + eta * a * p
    for (size_t n = 0; n < nx * ny; n++) {
      x[n] += eta * p[n];
      r[n] += eta * a_dot_p[n];
    }
    const double r_dot_r_old = r_dot_r;
    r_dot_r = compute_inner_product(nx * ny, r, r);
    const double theta = r_dot_r / r_dot_r_old;
    // p = r + theta * p
    for (size_t n = 0; n < nx * ny; n++) {
      p[n] = r[n] + theta * p[n];
    }
  }
  free(r);
  free(p);
  free(a_dot_p);
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
