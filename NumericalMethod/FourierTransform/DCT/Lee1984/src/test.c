#include <stdio.h>
#include <math.h>
#include "test.h"

// for test use, not needed to perform DCTs

static const double pi = 3.141592653589793238462643383;

// compute DCT type 2 naively in O(N^2)
int test_exec_f (
    const size_t nitems,
    const double * xs,
    double * ys
) {
  for (size_t j = 0; j < nitems; j++) {
    double * y = ys + j;
    *y = 0.;
    for (size_t i = 0; i < nitems; i++) {
      const double phase = pi * (2. * i + 1.) * j / (2. * nitems);
      *y += 2. * xs[i] * cos(phase);
    }
  }
  return 0;
}

// compute DCT type 3 naively in O(N^2)
int test_exec_b (
    const size_t nitems,
    const double * xs,
    double * ys
) {
  for (size_t j = 0; j < nitems; j++) {
    double * y = ys + j;
    *y = xs[0];
    for (size_t i = 1; i < nitems; i++) {
      const double phase = pi * (2. * j + 1.) * i / (2. * nitems);
      *y += 2. * xs[i] * cos(phase);
    }
  }
  return 0;
}

// compare two signals (L^1 norm)
int test_compare (
    const char message[],
    const size_t nitems,
    const double * y0s,
    const double * y1s
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

