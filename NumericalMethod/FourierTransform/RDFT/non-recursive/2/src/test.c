#include <stdlib.h>
#include <math.h>
#include <complex.h>
#include "test.h"

static const double pi = 3.141592653589793238462643383;

int rdft_naive_exec_f(
    const size_t nitems,
    double * const xs
) {
  double complex * const ys = malloc(nitems * sizeof(double complex));
  double complex * const zs = malloc(nitems * sizeof(double complex));
  for (size_t n = 0; n < nitems; n++) {
    ys[n] = xs[n] + 0. * I;
  }
  for (size_t k = 0; k < nitems; k++) {
    zs[k] = 0.;
    for (size_t n = 0; n < nitems; n++) {
      zs[k] += ys[n] * cexp(- 2. * pi * n * k * I / nitems);
    }
  }
  for (size_t k = 0; k < nitems; k++) {
    xs[k] = k < nitems / 2 + 1 ? creal(zs[k]) : cimag(zs[k]);
  }
  free(ys);
  free(zs);
  return 0;
}

int rdft_naive_exec_b(
    const size_t nitems,
    double * const xs
) {
  double complex * const ys = malloc(nitems * sizeof(double complex));
  double complex * const zs = malloc(nitems * sizeof(double complex));
  for (size_t k = 0; k < nitems; k++) {
    if (k == 0) {
      ys[k] = xs[k];
    } else if (k < nitems / 2) {
      ys[k] = xs[k] - xs[nitems - k] * I;
    } else if (k == nitems / 2) {
      ys[k] = xs[k];
    } else {
      ys[k] = xs[nitems - k] + xs[k] * I;
    }
  }
  for (size_t n = 0; n < nitems; n++) {
    zs[n] = 0.;
    for (size_t k = 0; k < nitems; k++) {
      zs[n] += ys[k] * cexp(+ 2. * pi * n * k * I / nitems);
    }
  }
  for (size_t n = 0; n < nitems; n++) {
    xs[n] = creal(zs[n]) / nitems;
  }
  free(ys);
  free(zs);
  return 0;
}

