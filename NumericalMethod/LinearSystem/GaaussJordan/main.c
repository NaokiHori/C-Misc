#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// gauss-jordan elimination to compute inversed matrix,
// which is used to solve a linear system

static int print_arr(const char title[], const size_t nitems, const double *arr){
  printf("---");
  printf("%s", title);
  printf("---\n");
  for(size_t i = 0; i < nitems; i++){
    for(size_t j = 0; j < nitems; j++){
      printf("% 7.2f%c", arr[i * nitems + j], nitems - 1 == j ? '\n' : ' ');
    }
  }
  printf("---");
  for(size_t n = 0; n < strlen(title); n++){
    printf("-");
  }
  printf("---\n");
  return 0;
}

static int print_vec(const char title[], const size_t nitems, const double *vec){
  printf("---");
  printf("%s", title);
  printf("---\n");
  for(size_t i = 0; i < nitems; i++){
    printf("% 7.2f%c", vec[i], nitems - 1 == i ? '\n' : ' ');
  }
  printf("---");
  for(size_t n = 0; n < strlen(title); n++){
    printf("-");
  }
  printf("---\n");
  return 0;
}

static int compute_inverse(const size_t nitems, double *arr, double *inv){
  // forward
  for(size_t i = 0; i < nitems; i++){
    // normalise i-th row
    const double d = arr[i * nitems + i];
    for(size_t j = 0; j < nitems; j++){
      arr[i * nitems + j] /= d;
      inv[i * nitems + j] /= d;
    }
    // eliminate lower part
    for(size_t ii = i + 1; ii < nitems; ii++){
      const double f = arr[ii * nitems + i] / arr[i * nitems + i];
      for(size_t j = 0; j < nitems; j++){
        arr[ii * nitems + j] -= f * arr[i * nitems + j];
        inv[ii * nitems + j] -= f * inv[i * nitems + j];
      }
    }
  }
  // backward
  for(size_t i = nitems - 1; i > 0; i--){
    // eliminate upper part
    for(size_t ii = 0; ii < i; ii++){
      const double f = arr[ii * nitems + i] / arr[i * nitems + i];
      for(size_t j = 0; j < nitems; j++){
        arr[ii * nitems + j] -= f * arr[i * nitems + j];
        inv[ii * nitems + j] -= f * inv[i * nitems + j];
      }
    }
  }
  return 0;
}

static int compute_mul(const size_t nitems, const double *arr, const double *b, double *x){
  for(size_t i = 0; i < nitems; i++){
    x[i] = 0.;
    for(size_t j = 0; j < nitems; j++){
      x[i] += arr[i * nitems + j] * b[j];
    }
  }
  return 0;
}

int main(void){
  const size_t nitems = 3;
  double *a   = calloc(nitems * nitems, sizeof(double));
  double *arr = calloc(nitems * nitems, sizeof(double));
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
  memcpy(arr, a, nitems * nitems * sizeof(double));
  double *inv = calloc(nitems * nitems, sizeof(double));
  for(size_t i = 0; i < nitems; i++){
    for(size_t j = 0; j < nitems; j++){
      inv[i * nitems + j] = i == j ? 1. : 0.;
    }
  }
  print_arr("a", nitems, arr);
  compute_inverse(nitems, arr, inv);
  print_arr("a^-1", nitems, inv);
  double *b = calloc(nitems, sizeof(double));
  double *x = calloc(nitems, sizeof(double));
  b[0] = 1.;
  b[1] = 2.;
  b[2] = 3.;
  compute_mul(nitems, inv, b, x);
  print_vec("x", nitems, x);
  double *c = calloc(nitems, sizeof(double));
  compute_mul(nitems, a, x, c);
  print_vec("b",  nitems, b);
  print_vec("Ax", nitems, c);
  free(a);
  free(arr);
  free(inv);
  free(b);
  free(x);
  free(c);
  return 0;
}

