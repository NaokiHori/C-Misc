#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "rdft.h"

static const double pi = 3.141592653589793238;

static int output(
    const char file_name[],
    const size_t nitems,
    const double * const xs,
    const double * const ys
) {
  FILE * const fp = fopen(file_name, "w");
  for (size_t i = 0; i < nitems; i++) {
    fprintf(fp, "% .7e % .7e\n", xs[i], ys[i]);
  }
  fclose(fp);
  return 0;
}

int main(
    void
) {
  const size_t nitems = 64;
  const size_t k_min = 4;
  const size_t k_max = 8;
  double * const xs = malloc(nitems * sizeof(double));
  double * const ys = malloc(nitems * sizeof(double));
  for (size_t i = 0; i < nitems; i++) {
    xs[i] = 1. * i / nitems;
  }
  for (size_t i = 0; i < nitems; i++) {
    ys[i] = 0.;
  }
  for (size_t k = k_min; k < k_max; k++) {
    const double amp = 1. * rand() / RAND_MAX / k;
    const double phase = 1. * rand() / RAND_MAX;
    for (size_t i = 0; i < nitems; i++) {
      ys[i] += amp * sin(2. * pi * (k * xs[i] + phase));
    }
  }
  output("position.dat", nitems, xs, ys);
  rdft_plan_t * plan = NULL;
  if (0 != rdft_init_plan(nitems, &plan)) {
    return 1;
  }
  rdft_exec_f(plan, ys);
  ys[0] = 0.;
  for (size_t k = 1; k < nitems / 2; k++) {
    const double real = + ys[         k];
    const double imag = - ys[nitems - k];
    ys[         k] = - 1. * k * imag;
    ys[nitems - k] = - 1. * k * real;
  }
  ys[nitems / 2] = 0.;
  rdft_exec_b(plan, ys);
  if (0 != rdft_destroy_plan(&plan)) {
    return 1;
  }
  output("derivative.dat", nitems, xs, ys);
  free(xs);
  free(ys);
  return 0;
}
