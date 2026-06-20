#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

static int solve(
    const size_t nitems,
    const double * const a,
    const double * const b,
    double * const x
) {
  const double tolerance = 1e-8;
  double * const r = malloc(nitems * sizeof(double));
  for (;;) {
    static size_t step = 0;
    // compute residual vector
    // r = a * x - b
    for (size_t i = 0; i < nitems; i++) {
      r[i] = - b[i];
      for (size_t j = 0; j < nitems; j++) {
        r[i] += a[i * nitems + j] * x[j];
      }
    }
    // check convergence
    double r_dot_r = 0.;
    for (size_t i = 0; i < nitems; i++) {
      r_dot_r += r[i] * r[i];
    }
    printf("step %zu error % .1e\n", step, r_dot_r);
    if (r_dot_r < tolerance) {
      break;
    }
    double r_dot_a_dot_r = 0;
    for (size_t i = 0; i < nitems; i++) {
      for (size_t j = 0; j < nitems; j++) {
        r_dot_a_dot_r += r[i] * a[i * nitems + j] * r[j];
      }
    }
    const double eta = - r_dot_r / r_dot_a_dot_r;
    // update x
    // x = x + eta * r
    for (size_t i = 0; i < nitems; i++) {
      x[i] += eta * r[i];
    }
    step += 1;
  }
  free(r);
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
