// https://en.wikipedia.org/wiki/Jacobi_eigenvalue_algorithm
// https://www.eng.niigata-u.ac.jp/~nomoto/16.html

#include <assert.h>
#include <complex.h>
#include <math.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

#define N 3

static int print_matrix(
    const double complex a[N][N]
) {
  for (size_t i = 0; i < N; i++) {
    printf("    ");
    for (size_t j = 0; j < N; j++) {
      printf(
          "(% .1e + % .1e * I)%s",
          creal(a[i][j]),
          cimag(a[i][j]),
          j == N - 1 ? "\n" : ", "
      );
    }
  }
  return 0;
}

static int init_matrix(
    double complex a[N][N]
) {
  // initialize (N x N) Hermitian matrix randomly
  for (size_t i = 0; i < N; i++) {
    for (size_t j = 0; j < N; j++) {
      // upper triangular elements: assign random number
      // lower triangular elements: complex-conjugate of upper triangle item
      if (i <= j) {
        const double real = - 0.5 + 1. * rand() / RAND_MAX;
        const double imag = - 0.5 + 1. * rand() / RAND_MAX;
        a[i][j] = real + imag * I;
      } else {
        a[i][j] = conj(a[j][i]);
      }
    }
  }
  return 0;
}

static int compute_rotation_matrix(
    const double complex a[N][N],
    size_t * const p,
    size_t * const q,
    double * const theta,
    double * const phi
) {
  const double PI = 3.1415926535897932;
  // pick up indices of the maximum (on complex plane) value
  double max = 0.;
  *p = SIZE_MAX;
  *q = SIZE_MAX;
  for (size_t i = 0; i < N; i++) {
    for (size_t j = 0; j < i; j++) {
      const double val = cabs(a[i][j]);
      if (max < val) {
        max = val;
        *p = i;
        *q = j;
      }
    }
  }
  // eq 30
  *theta = 0.25 * PI - 0.5 * atan2(a[*p][*p] - a[*q][*q], 2. * cabs(a[*p][*q]));
  *phi = - carg(a[*p][*q]);
  return 0;
}

static int update_matrix(
    const size_t p,
    const size_t q,
    const double theta,
    const double phi,
    double complex a[N][N]
) {
  const double s = sin(theta);
  const double c = cos(theta);
  const double complex exp_m = cexp(- I * phi);
  const double complex exp_p = cexp(+ I * phi);
  // for simplicity, define another matrix as a buffer (eq 17)
  double complex b[N][N];
  for (size_t i = 0; i < N; i++) {
    for (size_t j = 0; j < N; j++) {
      b[i][j] = a[i][j];
    }
  }
  for (size_t k = 0; k < N; k++) {
    if (k != p && k != q) {
      {
        // eq 18, 20
        const double complex val = + a[p][k] * c + a[q][k] * s * exp_m;
        b[p][k] = val ;
        b[k][p] = conj(val);
      }
      {
        // eq 19, 21
        const double complex val = - a[q][k] * c + a[p][k] * s * exp_p;
        b[q][k] = val ;
        b[k][q] = conj(val);
      }
    }
  }
  // eq 22
  b[p][p] =
    + a[p][p] * c * c
    + a[q][q] * s * s
    + a[p][q] * s * c * exp_p
    + a[q][p] * s * c * exp_m;
  // eq 23
  b[q][q] =
    + a[p][p] * s * s
    + a[q][q] * c * c
    - a[p][q] * s * c * exp_p
    - a[q][p] * s * c * exp_m;
  // eq 24, 25
  b[p][q] = 0.;
  b[q][p] = 0.;
  for (size_t i = 0; i < N; i++) {
    for (size_t j = 0; j < N; j++) {
      a[i][j] = b[i][j];
    }
  }
  return 0;
}

int main(
    void
) {
  double complex a[N][N];
  init_matrix(a);
  printf("initial matrix\n");
  print_matrix(a);
  // initial matrix should be Hermitian
  for (size_t i = 0; i < N; i++) {
    for (size_t j = 0; j < i; j++) {
      assert(a[i][j] == conj(a[j][i]));
    }
  }
  // update matrix iteratively
  for (;;) {
    size_t p = SIZE_MAX;
    size_t q = SIZE_MAX;
    double theta = 0.;
    double phi = 0.;
    compute_rotation_matrix(a, &p, &q, &theta, &phi);
    const double theta_min = 1.e-16;
    if (fabs(theta) < theta_min) {
      break;
    }
    update_matrix(p, q, theta, phi, a);
  }
  printf("final matrix\n");
  print_matrix(a);
  return 0;
}

