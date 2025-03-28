#include <math.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

static int solve(
    const int_fast32_t nitems,
    double * restrict const a,
    double * restrict const x
) {
  // solve A x = b by means of the Gaussian elimination
  //   where x stores the input and output
  // NOTE: zero-division checks are omitted, assuming rank(A) is nitems
  // forward elimination
  for (int_fast32_t i = 0; i < nitems; i++) {
    for (int_fast32_t ii = i + 1; ii < nitems; ii++) {
      const double v = a[ii * nitems + i] / a[i * nitems + i];
      for (int_fast32_t j = i + 1; j < nitems; j++) {
        a[ii * nitems + j] -= v * a[i * nitems + j];
      }
      x[ii] -= v * x[i];
    }
  }
  // backward substitution
  for (int_fast32_t i = nitems - 1; i >= 0; i--) {
    for (int_fast32_t j = i + 1; j < nitems; j++) {
      x[i] -= a[i * nitems + j] * x[j];
    }
    x[i] /= a[i * nitems + i];
  }
  return 0;
}

static int gauss_newton_fit(
    const size_t nitems,
    const size_t norders,
    const size_t niters,
    const double threshold,
    const double * restrict xs,
    const double * restrict ys,
    double * restrict coefs
) {
  // fix a polynomial
  //   f(x) = sum_{i=0}^{N-1} b_i * x^i
  double * restrict jacobi = calloc(norders * nitems, sizeof(double));
  double * restrict a = calloc(norders * norders, sizeof(double));
  double * restrict x = calloc(norders, sizeof(double));
  // residuals are
  //   r_i = y_i - b_3 * x_i^3 - b_2 * x_i^2 - b_1 * x_i - b_0
  // the Jacobian of r_i with respect to unknowns b_j:
  //   d r_i / d b_j,
  // which are given by
  //   j = 0: - 1
  //   j = 1: - x_i
  //   j = 2: - x_i^2
  //   j = 3: - x_i^3
  //   ...
  // which has "nitems" row times "norders" columns
  for (size_t i = 0; i < nitems; i++) {
    for (size_t j = 0; j < nitems; j++) {
      jacobi[i * norders + j] = - pow(xs[i], j);
    }
  }
  // linear system a x = b
  for (size_t i = 0; i < norders; i++) {
    for (size_t k = 0; k < nitems; k++) {
      for (size_t j = 0; j < norders; j++) {
        a[i * norders + j] +=
          jacobi[i * nitems + k] * jacobi[k * norders + j];
      }
    }
  }
  for (size_t iter = 0; iter < niters; iter++) {
    for (size_t i = 0; i < norders; i++) {
      x[i] = 0.;
      for (size_t j = 0; j < nitems; j++) {
        double r = 0.;
        r += ys[j];
        for (size_t k = 0; k < norders; k++) {
          r -= coefs[k] * pow(xs[j], k);
        }
        x[i] += -1. * jacobi[i * nitems + j] * r;
      }
    }
    solve((int_fast32_t)norders, a, x);
    double residual = 0.;
    for (size_t i = 0; i < norders; i++) {
      residual += fabs(x[i]);
      coefs[i] += x[i];
    }
    printf("iter %10zu rasidual % .1e\n", iter, residual);
    if (residual < threshold) {
      puts("converged");
      goto clean_up;
    }
  }
  puts("not converged");
clean_up:
  free(jacobi);
  free(a);
  free(x);
  return 0;
}

int main(
    void
) {
  const size_t nitems = 128;
  const size_t norders = 6;
  // input signal
  double * xs = calloc(nitems, sizeof(double));
  double * ys = calloc(nitems, sizeof(double));
  // coefficients c[0] + c[1] * x + c[2] * x^2 + ...
  double * coefs = calloc(norders, sizeof(double));
  for (size_t i = 0; i < nitems; i++) {
    const double pi = 3.14159265358979323846264;
    const double x = 1. * i / (nitems - 1);
    xs[i] = x;
    ys[i] = exp(-1. * x) * cos(2. * pi * x);
  }
  // try fitting
  // give an appropriate initial condition if necessary
  gauss_newton_fit(nitems, norders, 1 << 14, 1.e-14, xs, ys, coefs);
  // output result
  FILE * const fp = fopen("log.out", "w");
  for (size_t i = 0; i < nitems; i++) {
    const double x = xs[i];
    double z = 0.;
    for (size_t j = 0; j < norders; j++) {
      z += coefs[j] * pow(x, j);
    }
    fprintf(fp, "% .15e % .15e % .15e\n", xs[i], ys[i], z);
  }
  fclose(fp);
  // clean-up
  free(xs);
  free(ys);
  free(coefs);
  return 0;
}
