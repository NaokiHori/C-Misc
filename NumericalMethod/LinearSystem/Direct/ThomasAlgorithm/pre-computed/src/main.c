#include <math.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

static const double PI = 3.141592653589793238462643383;

static int setup(
    const size_t nitems,
    double * const c,
    double * const lower_diagonals,
    double * const upper_diagonals,
    double * const input,
    double * const expected
) {
  // helmholtz equation
  // dp2/dx2 - k^2 p = q
  const double k = 1.;
  const double dx = 1. / (nitems + 1);
  *c = pow(k, 2.);
  for (size_t i = 0; i < nitems; i++) {
    const double x = 1. * (i + 1) * dx;
    input[i] = - (pow(PI, 2.) + pow(k, 2.)) * sin(PI * x);
    expected[i] = sin(PI * x);
    lower_diagonals[i] = 1. / dx / dx;
    upper_diagonals[i] = 1. / dx / dx;
  }
  return 0;
}

static int solve(
    const size_t nitems,
    const double c,
    const double * const lower_diagonals,
    const double * const upper_diagonals,
    double * const inout
) {
  double * const k = malloc(nitems * sizeof(double));
  // rounding-error-mitigated formulation
  {
    double s = c + lower_diagonals[0];
    k[0] = - 1. / (upper_diagonals[0] + s);
    for (size_t i = 1; i < nitems; i++) {
      s = c + lower_diagonals[i] * (s / (s + upper_diagonals[i - 1]));
      k[i] = - 1. / (upper_diagonals[i] + s);
    }
  }
  // naive formulation (for comparison)
  // {
  //   k[0] = 1. / (- c - lower_diagonals[0] - upper_diagonals[0]);
  //   for (size_t i = 1; i < nitems; i++) {
  //     const double main_diagonal = - c - lower_diagonals[i] - upper_diagonals[i];
  //     k[i] = 1. / (main_diagonal - lower_diagonals[i] * upper_diagonals[i - 1] * k[i - 1]);
  //   }
  // }
  //
  inout[0] *= k[0];
  for (size_t i = 1; i < nitems; i++) {
    inout[i] = k[i] * fma(- lower_diagonals[i], inout[i - 1], inout[i]);
  }
  for (int i = nitems - 2; 0 <= i; i--) {
    inout[i] = fma(- upper_diagonals[i] * k[i], inout[i + 1], inout[i]);
  }
  free(k);
  return 0;
}

static int compare(
    const size_t nitems,
    const double * const output,
    const double * const expected
) {
  double errors[2] = {0., 0.};
  for (size_t i = 0; i < nitems; i++) {
    const double error = output[i] - expected[i];
    errors[0] += pow(error, 2.);
    errors[1] = fmax(errors[1], fabs(error));
  }
  errors[0] = sqrt(errors[0] / nitems);
  printf("%zu % .1e % .1e\n", nitems, errors[0], errors[1]);
  return 0;
}

int main(
    void
) {
  for (size_t nitems = 4; nitems < (1 << 24); nitems *= 2) {
    double * const inout = malloc(nitems * sizeof(double));
    double * const expected = malloc(nitems * sizeof(double));
    double * const lower_diagonals = malloc(nitems * sizeof(double));
    double * const upper_diagonals = malloc(nitems * sizeof(double));
    double c = 0.;
    setup(nitems, &c, lower_diagonals, upper_diagonals, inout, expected);
    solve(nitems, c, lower_diagonals, upper_diagonals, inout);
    compare(nitems, inout, expected);
    free(inout);
    free(expected);
    free(lower_diagonals);
    free(upper_diagonals);
  }
  return 0;
}

