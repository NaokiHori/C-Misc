#include "modified_cholesky.h"

// modified Cholesky decomposition to solve a linear system

int modified_cholesky (
    const size_t nitems,
    double * const a,
    double * const x
) {
  for (size_t i = 0; i < nitems; i++) {
    // lower triangular matrix
    for (size_t j = 0; j < i; j++) {
      for (size_t k = 0; k < j; k++) {
        const double lik = a[i * nitems + k];
        const double ljk = a[j * nitems + k];
        const double dk  = a[k * nitems + k];
        a[i * nitems + j] -= lik * ljk * dk;
      }
      a[i * nitems + j] /= a[j * nitems + j];
    }
    // diagonal component
    for (size_t j = 0; j < i; j++) {
      const double dj  = a[j * nitems + j];
      const double lij = a[i * nitems + j];
      a[i * nitems + i] -= lij * lij * dj;
    }
  }
  // L y = b
  for (size_t i = 0; i < nitems; i++) {
    for (size_t j = 0; j < i; j++) {
      x[i] -= a[i * nitems + j] * x[j];
    }
  }
  // D z = y
  for (size_t i = 0; i < nitems; i++) {
    x[i] /= a[i * nitems + i];
  }
  // LT x = z
  for (size_t i = nitems - 1; ; i--) {
    for (size_t j = i + 1; j < nitems; j++) {
      x[i] -= a[j * nitems + i] * x[j];
    }
    if (0 == i) {
      break;
    }
  }
  return 0;
}

