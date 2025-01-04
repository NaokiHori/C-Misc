#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <complex.h>
#include "dft.h"

static int naive_dft (
    const size_t nitems,
    double complex * const xs
) {
  const double pi = 3.14159265358979324;
  double complex * const ys = malloc(nitems * sizeof(double complex));
  for (size_t k = 0; k < nitems; k++) {
    double complex * const y = ys + k;
    *y = 0. + I * 0.;
    for (size_t n = 0; n < nitems; n++) {
      *y += xs[n] * cexp(- 2. * pi * n * k * I / nitems);
    }
  }
  for (size_t k = 0; k < nitems; k++) {
    xs[k] = ys[k];
  }
  free(ys);
  return 0;
}

static int naive_idft (
    const size_t nitems,
    double complex * const xs
) {
  const double pi = 3.14159265358979324;
  double complex * const ys = malloc(nitems * sizeof(double complex));
  for (size_t n = 0; n < nitems; n++) {
    double complex * const y = ys + n;
    *y = 0. + I * 0.;
    for (size_t k = 0; k < nitems; k++) {
      *y += xs[k] * cexp(2. * pi * n * k * I / nitems);
    }
    *y /= nitems;
  }
  for (size_t n = 0; n < nitems; n++) {
    xs[n] = ys[n];
  }
  free(ys);
  return 0;
}

// dft followed by idft should recover original
static int test0 (
    const size_t nitems
) {
  double complex * const xs = malloc(nitems * sizeof(double complex));
  double complex * const ys = malloc(nitems * sizeof(double complex));
  for (size_t i = 0; i < nitems; i++) {
    const double real = - 0.5 + 1. * rand() / RAND_MAX;
    const double imag = - 0.5 + 1. * rand() / RAND_MAX;
    xs[i] = real + I * imag;
    ys[i] = real + I * imag;
  }
  dft_plan_t * plan = NULL;
  if (0 != dft_init_plan(nitems, &plan)) {
    puts("failed to init a plan");
    return 1;
  }
  if (0 != dft_exec_f(plan, xs)) {
    puts("forward execution failed");
    goto abort;
  }
  if (0 != dft_exec_b(plan, xs)) {
    puts("backward execution failed");
    goto abort;
  }
  double dif = 0.;
  for (size_t i = 0; i < nitems; i++) {
    dif += cabs(ys[i] - xs[i]);
  }
  dif /= 1. * nitems;
  printf("DFT followed by iDFT: %zu % .15e\n", nitems, dif);
abort:
  free(xs);
  free(ys);
  if (0 != dft_destroy_plan(&plan)) {
    puts("failed to destroy a plan");
    return 1;
  }
  return 0;
}

// compare dft with naive implementation
static int test1 (
    const size_t nitems
) {
  double complex * const xs = malloc(nitems * sizeof(double complex));
  double complex * const ys = malloc(nitems * sizeof(double complex));
  for (size_t i = 0; i < nitems; i++) {
    const double real = - 0.5 + 1. * rand() / RAND_MAX;
    const double imag = - 0.5 + 1. * rand() / RAND_MAX;
    xs[i] = real + I * imag;
    ys[i] = real + I * imag;
  }
  dft_plan_t * plan = NULL;
  if (0 != dft_init_plan(nitems, &plan)) {
    puts("failed to init a plan");
    return 1;
  }
  if (0 != dft_exec_f(plan, xs)) {
    puts("forward execution failed");
    goto abort;
  }
  naive_dft(nitems, ys);
  double dif = 0.;
  for (size_t i = 0; i < nitems; i++) {
    dif += cabs(ys[i] - xs[i]);
  }
  dif /= 1. * nitems;
  printf("DFT compared with naive impl.: %zu % .15e\n", nitems, dif);
abort:
  free(xs);
  free(ys);
  if (0 != dft_destroy_plan(&plan)) {
    puts("failed to destroy a plan");
    return 1;
  }
  return 0;
}

// compare idft with naive implementation
static int test2 (
    const size_t nitems
) {
  double complex * const xs = malloc(nitems * sizeof(double complex));
  double complex * const ys = malloc(nitems * sizeof(double complex));
  for (size_t i = 0; i < nitems; i++) {
    const double real = (- 0.5 + 1. * rand() / RAND_MAX) * nitems;
    const double imag = (- 0.5 + 1. * rand() / RAND_MAX) * nitems;
    xs[i] = real + I * imag;
    ys[i] = real + I * imag;
  }
  dft_plan_t * plan = NULL;
  if (0 != dft_init_plan(nitems, &plan)) {
    puts("failed to init a plan");
    return 1;
  }
  if (0 != dft_exec_b(plan, xs)) {
    puts("backward execution failed");
    goto abort;
  }
  naive_idft(nitems, ys);
  double dif = 0.;
  for (size_t i = 0; i < nitems; i++) {
    dif += cabs(ys[i] - xs[i]);
  }
  dif /= 1. * nitems;
  printf("iDFT compared with naive impl.: %zu % .15e\n", nitems, dif);
abort:
  free(xs);
  free(ys);
  if (0 != dft_destroy_plan(&plan)) {
    puts("failed to destroy a plan");
    return 1;
  }
  return 0;
}

int main (
    void
) {
  int retval = 0;
  const size_t nitems_list[] = {
    1,  2,  4,   8,  16,   32,   64, 128,  256,  512, 1024,
    3,  6, 12,  24,  48,   96,  192, 384,  768, 1536, 3072,
    5, 10, 20,  40,  80,  160,  320, 640, 1280, 2560, 5120,
    9, 27, 81, 243, 729, 2187, 6561,
  };
  for (size_t n = 0; n < sizeof(nitems_list) / sizeof(nitems_list[0]); n++) {
    retval += test0(nitems_list[n]);
  }
  for (size_t n = 0; n < sizeof(nitems_list) / sizeof(nitems_list[0]); n++) {
    retval += test1(nitems_list[n]);
  }
  for (size_t n = 0; n < sizeof(nitems_list) / sizeof(nitems_list[0]); n++) {
    retval += test2(nitems_list[n]);
  }
  return retval;
}

