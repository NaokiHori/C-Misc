#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// gauss elimination to solve a linear system

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

static int solve(const int_fast32_t nitems, double * restrict a, double * restrict x){
  // solve A x = b by means of the Gaussian elimination
  //   where x stores the input and output
  // NOTE: zero-division checks are omitted, assuming rank(A) is nitems
  // forward elimination
  for(int_fast32_t i = 0; i < nitems; i++){
    for(int_fast32_t ii = i + 1; ii < nitems; ii++){
      const double v = a[ii * nitems + i] / a[i * nitems + i];
      for(int_fast32_t j = i + 1; j < nitems; j++){
        a[ii * nitems + j] -= v * a[i * nitems + j];
      }
      x[ii] -= v * x[i];
    }
  }
  // backward substitution
  for(int_fast32_t i = nitems - 1; i >= 0; i--){
    for(int_fast32_t j = i + 1; j < nitems; j++){
      x[i] -= a[i * nitems + j] * x[j];
    }
    x[i] /= a[i * nitems + i];
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
  print_arr("A", nitems, a);
  double *b = calloc(nitems, sizeof(double));
  double *x = calloc(nitems, sizeof(double));
  b[0] = 1.;
  b[1] = 2.;
  b[2] = 3.;
  memcpy(x, b, nitems * sizeof(double));
  print_vec("b",  nitems, b);
  solve(nitems, arr, x);
  print_vec("x", nitems, x);
  double *c = calloc(nitems, sizeof(double));
  compute_mul(nitems, a, x, c);
  print_vec("Ax", nitems, c);
  free(a);
  free(arr);
  free(b);
  free(x);
  free(c);
  return 0;
}

