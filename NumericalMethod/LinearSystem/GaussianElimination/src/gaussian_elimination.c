#include "gaussian_elimination.h"

// gaussian elimination to solve a linear system

int gaussian_elimination (
    const size_t nitems,
    double * const a,
    double * const x
) {
  // forward sweep
  for (size_t i = 0; i < nitems; i++) {
    // normalize i-th row | 5
    const double f = 1. / a[i * nitems + i];
    for (size_t j = i; j < nitems; j++) {
      a[i * nitems + j] *= f;
    }
    x[i] *= f;
    // eliminate lower-triangular part | 7
    for (size_t ii = i + 1; ii < nitems; ii++) {
      const double f = a[ii * nitems + i];
      for (size_t j = i + 1; j < nitems; j++) {
        a[ii * nitems + j] -= f * a[i * nitems + j];
      }
      x[ii] -= f * x[i];
    }
  }
  // backward substitution | 8
  for (size_t i = nitems - 1; ; i--) {
    for (size_t j = i + 1; j < nitems; j++) {
      x[i] -= a[i * nitems + j] * x[j];
    }
    if (0 == i) {
      break;
    }
  }
  return 0;
}

