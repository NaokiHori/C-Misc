#include <stdio.h>
#include <math.h>
#include "test.h"

// for test use, not needed to perform DSTs

static const double pi = 3.141592653589793238462643383;

// compute DST type 2 naively in O(N^2)
int test_exec_f (
    const size_t nitems,
    const double * const xs,
    double * const ys
) {
  for (size_t k = 0; k < nitems; k++) {
    double * const y = ys + k;
    *y = 0.;
    for (size_t n = 0; n < nitems; n++) {
      const double phase = 2. * pi * (n + 0.5) * (k + 1) / (2. * nitems);
      *y += 2. * xs[n] * sin(phase);
    }
  }
  return 0;
}

// compute DST type 3 naively in O(N^2)
int test_exec_b (
    const size_t nitems,
    const double * const xs,
    double * const ys
) {
  for (size_t n = 0; n < nitems; n++) {
    double * const y = ys + n;
    *y = 0.;
    for (size_t k = 0; k < nitems; k++) {
      const double phase = 2. * pi * (n + 0.5) * (k + 1) / (2. * nitems);
      const double x = nitems - 1 == k ? 0.5 * xs[k] : xs[k];
      *y += x * sin(phase);
    }
    *y /= 1. * nitems;
  }
  return 0;
}

// compare two signals (L^1 norm)
int test_compare (
    const char message[],
    const size_t nitems,
    const double * const y0s,
    const double * const y1s
) {
  double residual = 0.;
  for (size_t i = 0; i < nitems; i++) {
    residual += fabs(y1s[i] - y0s[i]) / nitems;
  }
  printf(
      "%s, nitems: %zu, residual (L^1): % 4.1e\n",
      message, nitems, residual
  );
  return 0;
}

