#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

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

static int solve(
    const size_t nitems,
    const double * const a,
    const double * const b,
    double * const x
) {
  const double tolerance = 1e-8;
  double * const r = malloc(nitems * sizeof(double));
  double * const p = malloc(nitems * sizeof(double));
  double * const a_dot_p = malloc(nitems * sizeof(double));
  // r = a * x - b
  for (size_t i = 0; i < nitems; i++) {
    r[i] = compute_inner_product(nitems, a + i * nitems, x) - b[i];
  }
  for (size_t i = 0; i < nitems; i++) {
    p[i] = r[i];
  }
  double r_dot_r = compute_inner_product(nitems, r, r);
  for (size_t step = 0; ; step += 1) {
    // check convergence
    printf("step %zu error % .1e\n", step, r_dot_r);
    if (r_dot_r < tolerance) {
      break;
    }
    // compute eta
    for (size_t i = 0; i < nitems; i++) {
      a_dot_p[i] = compute_inner_product(nitems, a + i * nitems, p);
    }
    const double p_dot_a_dot_p = compute_inner_product(nitems, p, a_dot_p);
    const double eta = - r_dot_r / p_dot_a_dot_p;
    // x = x + eta * p
    // r = r + eta * a * p
    for (size_t i = 0; i < nitems; i++) {
      x[i] += eta * p[i];
      r[i] += eta * a_dot_p[i];
    }
    const double r_dot_r_old = r_dot_r;
    r_dot_r = compute_inner_product(nitems, r, r);
    const double theta = r_dot_r / r_dot_r_old;
    // p = r + theta * p
    for (size_t i = 0; i < nitems; i++) {
      p[i] = r[i] + theta * p[i];
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
#define NITEMS 2
  const double a[NITEMS * NITEMS] = {2., 2., 2., 3.};
  const double b[NITEMS] = {1., -1.};
  double x[NITEMS] = {1., 1.};
  if (0 != solve(NITEMS, a, b, x)) {
    return 1;
  }
  for (size_t i = 0; i < NITEMS; i++) {
    printf("%zu % .7e\n", i, x[i]);
  }
  return 0;
}
