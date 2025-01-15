#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "rdft.h"
#include "ref.h"
#include "test.h"

static const double pi = 3.141592653589793238462643383;

static int set_values(
    const size_t nitems,
    double * const xs,
    double * const ys
) {
  for (size_t i = 0; i < nitems; i++) {
    xs[i] = sin(2. * pi * i / nitems) + 1. * rand() / RAND_MAX;
    ys[i] = xs[i];
  }
  return 0;
}

static int test_0(
    const size_t nitems,
    rdft_plan_t * const plan,
    double * const xs,
    double * const ys
) {
  set_values(nitems, xs, ys);
  rdft_exec_f(plan, xs);
  rdft_exec_b(plan, xs);
  double error = 0.;
  for (size_t i = 0; i < nitems; i++) {
    error += pow(xs[i] - ys[i], 2.);
  }
  error = sqrt(error / nitems);
  printf("residual: % .7e\n", error);
  return 0;
}

static int test_1(
    const size_t nitems,
    rdft_plan_t * const plan,
    double * const xs,
    double * const ys
) {
  set_values(nitems, xs, ys);
  rdft_exec_f(plan, xs);
  rdft_naive_exec_f(nitems, ys);
  double error = 0.;
  for (size_t i = 0; i < nitems; i++) {
    error += pow(xs[i] - ys[i], 2.);
  }
  error = sqrt(error / nitems);
  printf("residual: % .7e\n", error);
  return 0;
}

static int test_2(
    const size_t nitems,
    rdft_plan_t * const plan,
    double * const xs,
    double * const ys
) {
  set_values(nitems, xs, ys);
  rdft_exec_b(plan, xs);
  rdft_naive_exec_b(nitems, ys);
  double error = 0.;
  for (size_t i = 0; i < nitems; i++) {
    error += pow(xs[i] - ys[i], 2.);
  }
  error = sqrt(error / nitems);
  printf("residual: % .7e\n", error);
  return 0;
}

int main(
    void
) {
  const size_t nitems = 512;
  rdft_plan_t * plan = NULL;
  if (0 != rdft_init_plan(nitems, &plan)) {
    return 1;
  }
  double * const xs = malloc(nitems * sizeof(double));
  double * const ys = malloc(nitems * sizeof(double));
  test_0(nitems, plan, xs, ys);
  test_1(nitems, plan, xs, ys);
  test_2(nitems, plan, xs, ys);
  free(xs);
  free(ys);
  if (0 != rdft_destroy_plan(&plan)) {
    return 1;
  }
  return 0;
}
