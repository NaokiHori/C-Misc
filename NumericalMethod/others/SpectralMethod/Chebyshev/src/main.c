#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <fftw3.h>

static const double pi = 3.14159265358979323846;

// compute Chebyshev coefficients of the derivative
static int compute_derivative (
    const size_t nitems,
    double * const ys
) {
  if (nitems < 2) {
    printf("nitems (%zu) should be larger than 1\n", nitems);
    return 1;
  }
  // go to frequency domain
  {
    fftw_plan const plan = fftw_plan_r2r_1d(nitems + 1, ys, ys, FFTW_REDFT00, FFTW_ESTIMATE);
    fftw_execute(plan);
    fftw_destroy_plan(plan);
  }
  // compute derivative following a recurrence relation
  {
    // c_k^' = 2 (k+1) c_{k+1} + c_{k+2}^'
    double * const zs = calloc(nitems + 1, sizeof(double));
    size_t k = nitems - 1;
    zs[k] = 2. * (k + 1) * ys[k + 1];
    while (1) {
      k = k - 1;
      zs[k] = 2. * (k + 1) * ys[k + 1] + zs[k + 2];
      if (0 == k) {
        break;
      }
    }
    for (size_t k = 0; k < nitems + 1; k++) {
      ys[k] = zs[k];
    }
    free(zs);
  }
  // go back to physical domain to obtain nodal values
  {
    fftw_plan const plan = fftw_plan_r2r_1d(nitems + 1, ys, ys, FFTW_REDFT00, FFTW_ESTIMATE);
    fftw_execute(plan);
    fftw_destroy_plan(plan);
    for (size_t n = 0; n < nitems + 1; n++) {
      ys[n] /= 2. * nitems;
    }
  }
  return 0;
}

static int output (
    const size_t nitems,
    const double * const xs,
    const double * const y1s,
    const double * const z1s,
    const double * const y2s,
    const double * const z2s
) {
  FILE * const fp = fopen("result.dat", "w");
  for (size_t n = 0; n < nitems + 1; n++) {
    fprintf(fp, "% .15e % .15e % .15e % .15e % .15e\n", xs[n], y1s[n], z1s[n], y2s[n], z2s[n]);
  }
  fclose(fp);
  return 0;
}

int main (
    void
) {
  const size_t nitems = 128;
  double * const xs = calloc(nitems + 1, sizeof(double));
  double * const ys[2] = {
    calloc(nitems + 1, sizeof(double)),
    calloc(nitems + 1, sizeof(double)),
  };
  double * const zs[2] = {
    calloc(nitems + 1, sizeof(double)),
    calloc(nitems + 1, sizeof(double)),
  };
  // initialize input (y) and answers (z)
  // consider y = f(x) and its 1st-order / 2nd-order derivatives
  for (size_t n = 0; n < nitems + 1; n++) {
    const double x = cos(1. * n * pi / nitems);
    xs[n] = x;
    // input: f(x) = 1 / (1 + 0.25 cos(4x + 2))
    ys[0][n] = 1. / (1. + 0.25 * cos(4. * x + 2.));
    // answer: df/dx
    zs[0][n] = sin(4. * x + 2.) / pow(1. + 0.25 * cos(4. * x + 2.), 2.);
    // answer: d2f/dx2
    zs[1][n] = (256. * cos(2. + 4. * x) + 64. * pow(cos(2. + 4. * x), 2.) + 128. * pow(sin(2. + 4. * x), 2.)) / pow((4. + cos(2. + 4. * x)), 3.);
  }
  compute_derivative(nitems, ys[0]);
  // copy 1st-order derivative and repeat the same procedure
  //   to compute 2nd-order derivative
  for (size_t n = 0; n < nitems + 1; n++) {
    ys[1][n] = ys[0][n];
  }
  compute_derivative(nitems, ys[1]);
  // output answer and result
  output(nitems, xs, ys[0], zs[0], ys[1], zs[1]);
  // clean-up
  free(xs);
  free(ys[0]);
  free(ys[1]);
  free(zs[0]);
  free(zs[1]);
  fftw_cleanup();
  return 0;
}

