#include <stdio.h>
#include "gauss_jordan.h"

// gauss-jordan elimination to compute inversed matrix,
// which is used to solve a linear system

int gauss_jordan (
    const size_t nitems,
    double * const arr,
    double * const inv
) {
  // initialize inverse matrix as an identity matrix
  for (size_t i = 0; i < nitems; i++) {
    for (size_t j = 0; j < nitems; j++) {
      inv[i * nitems + j] = i == j ? 1. : 0.;
    }
  }
  // forward sweep
  for (size_t i = 0; i < nitems; i++) {
    // normalize i-th row | 8
    const double f = 1. / arr[i * nitems + i];
    arr[i * nitems + i] = 1.;
    for (size_t j = i + 1; j < nitems; j++) {
      arr[i * nitems + j] *= f;
    }
    for (size_t j = 0; j < i + 1; j++) {
      inv[i * nitems + j] *= f;
    }
    // eliminate lower-triangular part | 9
    for (size_t ii = i + 1; ii < nitems; ii++) {
      const double f = arr[ii * nitems + i];
      for (size_t j = i; j < nitems; j++) {
        arr[ii * nitems + j] -= f * arr[i * nitems + j];
      }
      for (size_t j = 0; j < i + 1; j++) {
        inv[ii * nitems + j] -= f * inv[i * nitems + j];
      }
    }
  }
  // backward substitution
  for (size_t i = nitems - 1; ; i--) {
    // eliminate upper-triangular part | 7
    for (size_t ii = 0; ii < i; ii++) {
      const double f = arr[ii * nitems + i];
      arr[ii * nitems + i] = 0.;
      for (size_t j = 0; j < nitems; j++) {
        inv[ii * nitems + j] -= f * inv[i * nitems + j];
      }
    }
    if (0 == i) {
      break;
    }
  }
  return 0;
}

