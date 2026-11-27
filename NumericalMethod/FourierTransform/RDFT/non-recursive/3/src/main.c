#include <complex.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include "rdft.h"

static const double pi = 3.141592653589793238462643383;

static int naive_forward_rdft(
    const size_t nitems,
    double * const xs
) {
  double complex * const ys = malloc(nitems * sizeof(double complex));
  double complex * const zs = malloc(nitems * sizeof(double complex));
  for (size_t n = 0; n < nitems; n++) {
    ys[n] = xs[n] + 0. * I;
  }
  for (size_t k = 0; k < nitems; k++) {
    zs[k] = 0.;
    for (size_t n = 0; n < nitems; n++) {
      zs[k] += ys[n] * cexp(- 2. * pi * n * k * I / nitems);
    }
  }
  for (size_t k = 0; k < nitems; k++) {
    xs[k] = k < nitems / 2 + 1 ? creal(zs[k]) : cimag(zs[k]);
  }
  free(ys);
  free(zs);
  return 0;
}

static int naive_backward_rdft(
    const size_t nitems,
    double * const xs
) {
  double complex * const ys = malloc(nitems * sizeof(double complex));
  double complex * const zs = malloc(nitems * sizeof(double complex));
  for (size_t k = 0; k < nitems; k++) {
    if (k == 0) {
      ys[k] = xs[k];
    } else if (k < nitems / 2) {
      ys[k] = xs[k] - xs[nitems - k] * I;
    } else if (k == nitems / 2) {
      ys[k] = xs[k];
    } else {
      ys[k] = xs[nitems - k] + xs[k] * I;
    }
  }
  for (size_t n = 0; n < nitems; n++) {
    zs[n] = 0.;
    for (size_t k = 0; k < nitems; k++) {
      zs[n] += ys[k] * cexp(+ 2. * pi * n * k * I / nitems);
    }
  }
  for (size_t n = 0; n < nitems; n++) {
    xs[n] = creal(zs[n]) / nitems;
  }
  free(ys);
  free(zs);
  return 0;
}

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

static int test_forward(
    const size_t nitems
) {
  rdft_plan_t * plan = NULL;
  if (0 != rdft_init_plan(nitems, &plan)) {
    return 1;
  }
  double * const xs = malloc(nitems * sizeof(double));
  double * const ys = malloc(nitems * sizeof(double));
  set_values(nitems, xs, ys);
  rdft_execute_inplace_forward_transform(plan, xs);
  naive_forward_rdft(nitems, ys);
  double error = 0.;
  for (size_t i = 0; i < nitems; i++) {
    error += pow(xs[i] - ys[i], 2.);
  }
  error = sqrt(error / nitems);
  printf("% .1e\n", error);
  free(xs);
  free(ys);
  rdft_destroy_plan(&plan);
  return 0;
}

static int test_backward(
    const size_t nitems
) {
  rdft_plan_t * plan = NULL;
  if (0 != rdft_init_plan(nitems, &plan)) {
    return 1;
  }
  double * const xs = malloc(nitems * sizeof(double));
  double * const ys = malloc(nitems * sizeof(double));
  set_values(nitems, xs, ys);
  rdft_execute_inplace_backward_transform(plan, xs);
  naive_backward_rdft(nitems, ys);
  double error = 0.;
  for (size_t i = 0; i < nitems; i++) {
    error += pow(xs[i] - ys[i], 2.);
  }
  error = sqrt(error / nitems);
  printf("% .1e\n", error);
  free(xs);
  free(ys);
  rdft_destroy_plan(&plan);
  return 0;
}

static int test_roundtrip(
    const size_t nitems
) {
  rdft_plan_t * plan = NULL;
  if (0 != rdft_init_plan(nitems, &plan)) {
    return 1;
  }
  double * const xs = malloc(nitems * sizeof(double));
  double * const ys = malloc(nitems * sizeof(double));
  set_values(nitems, xs, ys);
  rdft_execute_inplace_forward_transform(plan, xs);
  rdft_execute_inplace_backward_transform(plan, xs);
  double error = 0.;
  for (size_t i = 0; i < nitems; i++) {
    error += pow(xs[i] - ys[i], 2.);
  }
  error = sqrt(error / nitems);
  printf("% .1e\n", error);
  free(xs);
  free(ys);
  rdft_destroy_plan(&plan);
  return 0;
}

static int test_invalid_plan(
    const size_t nitems
) {
  // expect to fail gracefully
  rdft_plan_t * plan = NULL;
  rdft_init_plan(nitems, &plan);
  rdft_destroy_plan(&plan);
  return 0;
}

int main(
    void
) {
  const size_t nitems_list[] = {
    2, 4, 6, 8, 12, 16, 30, 64, 128, 192,
    256, 384, 512, 768, 1024,
  };
  for (size_t n = 0; n < sizeof(nitems_list) / sizeof(size_t); n++) {
    const size_t nitems = nitems_list[n];
    test_forward(nitems);
    test_backward(nitems);
    test_roundtrip(nitems);
  }
  test_invalid_plan(3);
  return 0;
}

