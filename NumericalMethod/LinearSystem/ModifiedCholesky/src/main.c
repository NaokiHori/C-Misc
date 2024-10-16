#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "modified_cholesky.h"

static int print_arr (
    const char title[],
    const size_t nitems,
    const double * const arr
) {
  printf("---");
  printf("%s", title);
  printf("---\n");
  for (size_t i = 0; i < nitems; i++) {
    for (size_t j = 0; j < nitems; j++) {
      printf("% 7.2f%c", arr[i * nitems + j], nitems - 1 == j ? '\n' : ' ');
    }
  }
  printf("---");
  for (size_t n = 0; n < strlen(title); n++) {
    printf("-");
  }
  printf("---\n");
  return 0;
}

static int print_vec (
    const char title[],
    const size_t nitems,
    const double * const vec
) {
  printf("---");
  printf("%s", title);
  printf("---\n");
  for (size_t i = 0; i < nitems; i++) {
    printf("% 7.2f%c", vec[i], nitems - 1 == i ? '\n' : ' ');
  }
  printf("---");
  for (size_t n = 0; n < strlen(title); n++) {
    printf("-");
  }
  printf("---\n");
  return 0;
}

static int compute_mul (
    const size_t nitems,
    const double * const arr,
    const double * const b,
    double * const x
) {
  for (size_t i = 0; i < nitems; i++) {
    x[i] = 0.;
    for (size_t j = 0; j < nitems; j++) {
      x[i] += arr[i * nitems + j] * b[j];
    }
  }
  return 0;
}

int main (
    void
) {
  const size_t nitems = 3;
  double * const a   = calloc(nitems * nitems, sizeof(double));
  double * const arr = calloc(nitems * nitems, sizeof(double));
  // consider a symetric and positive-definite matrix
  // https://en.wikipedia.org/wiki/Cholesky_decomposition#Example
  // +-----+-----+-----+
  // | + 4 | +12 | -16 |
  // +-----+-----+-----+
  // | +12 | +37 | -43 |
  // +-----+-----+-----+
  // | -16 | -43 | +98 |
  // +-----+-----+-----+
  a[0 * nitems + 0] = + 4.;
  a[0 * nitems + 1] = +12.;
  a[0 * nitems + 2] = -16.;
  a[1 * nitems + 0] = +12.;
  a[1 * nitems + 1] = +37.;
  a[1 * nitems + 2] = -43.;
  a[2 * nitems + 0] = -16.;
  a[2 * nitems + 1] = -43.;
  a[2 * nitems + 2] = +98.;
  for (size_t n = 0; n < nitems * nitems; n++) {
    arr[n] = a[n];
  }
  print_arr("a", nitems, arr);
  double * const b = calloc(nitems, sizeof(double));
  double * const x = calloc(nitems, sizeof(double));
  b[0] = 1.;
  b[1] = 2.;
  b[2] = 3.;
  for (size_t n = 0; n < nitems; n++) {
    x[n] = b[n];
  }
  modified_cholesky(nitems, arr, x);
  print_vec("x", nitems, x);
  double * const c = calloc(nitems, sizeof(double));
  compute_mul(nitems, a, x, c);
  print_vec("b",  nitems, b);
  print_vec("Ax", nitems, c);
  free(a);
  free(arr);
  free(b);
  free(x);
  free(c);
  return 0;
}

