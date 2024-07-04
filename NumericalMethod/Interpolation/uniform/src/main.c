#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include <errno.h>
#include "interp_spectral.h"

static const double pi = 3.14159265358979324;

static int output (
    const char fname[],
    const size_t n,
    const double * xs,
    const double * ys
) {
  errno = 0;
  FILE * fp = fopen(fname, "w");
  if (NULL == fp) {
    perror(fname);
    return 1;
  }
  for (size_t i = 0; i < n; i++) {
    fprintf(fp, "% .15e % .15e\n", xs[i], ys[i]);
  }
  fclose(fp);
  return 0;
}

static double func (
    const double x
) {
  return 1. / (5. - 4. * cos(2. * pi * x + 2.));
}

int main (
    void
) {
  const size_t n0 = 37;
  const size_t n1 = 97;
  interp_spectral_plan_t * plan = NULL;
  double * const x0s = malloc(n0 * sizeof(double));
  double * const x1s = malloc(n1 * sizeof(double));
  double * const y0s = malloc(n0 * sizeof(double));
  double * const y1s = malloc(n1 * sizeof(double));
  // coordinates, before and after, which are used to initialise interpolator
  for (size_t i = 0; i < n0; i++) {
    x0s[i] = 1. * i / n0;
  }
  for (size_t i = 0; i < n1; i++) {
    x1s[i] = 1. * i / n1;
  }
  if (0 != interp_spectral_initialise(n0, n1, &plan)) {
    goto abort;
  }
  // function values, before and after
  // set "before" and perform interpolation
  for (size_t i = 0; i < n0; i++) {
    y0s[i] = func(x0s[i]);
  }
  if (0 != interp_spectral_execute(plan, y0s, y1s)) {
    goto abort;
  }
  if (0 != interp_spectral_finalise(&plan)) {
    goto abort;
  }
  // dump functions before and after interpolation
  output("bef.dat", n0, x0s, y0s);
  output("aft.dat", n1, x1s, y1s);
abort:
  free(x0s);
  free(x1s);
  free(y0s);
  free(y1s);
  return 0;
}

