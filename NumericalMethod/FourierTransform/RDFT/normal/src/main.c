#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <complex.h>
#include "rdft.h"

static int naive_dft (
    const size_t nitems,
    const double complex * xs,
    double complex * ys
) {
  const double pi = 3.14159265358979324;
  for (size_t k = 0; k < nitems; k++) {
    double complex * y = ys + k;
    *y = 0. + I * 0.;
    for (size_t n = 0; n < nitems; n++) {
      *y += xs[n] * cexp(- 2. * pi * n * k * I / nitems);
    }
  }
  return 0;
}

static int naive_idft (
    const size_t nitems,
    const double complex * xs,
    double complex * ys
) {
  const double pi = 3.14159265358979324;
  for (size_t k = 0; k < nitems; k++) {
    double complex * y = ys + k;
    *y = 0. + I * 0.;
    for (size_t n = 0; n < nitems; n++) {
      *y += xs[n] * cexp(+ 2. * pi * n * k * I / nitems);
    }
    *y /= nitems;
  }
  return 0;
}

// rdft followed by irdft should recover original
static int test0 (
    const size_t nitems
) {
  double * const xs = malloc(nitems * sizeof(double));
  double * const ys = malloc(nitems * sizeof(double));
  for (size_t i = 0; i < nitems; i++) {
    const double value = - 0.5 + 1. * rand() / RAND_MAX;
    xs[i] = value;
    ys[i] = value;
  }
  rdft_plan_t * plan = NULL;
  if (0 != rdft_init_plan(nitems, &plan)) {
    puts("failed to init a plan");
    return 1;
  }
  if (0 != rdft_exec_f(plan, xs)) {
    puts("forward execution failed");
    goto abort;
  }
  if (0 != rdft_exec_b(plan, xs)) {
    puts("backward execution failed");
    goto abort;
  }
  double dif = 0.;
  for (size_t i = 0; i < nitems; i++) {
    dif += fabs(ys[i] - xs[i]);
  }
  dif /= 1. * nitems;
  printf("%zu % .15e\n", nitems, dif);
abort:
  free(xs);
  free(ys);
  if (0 != rdft_destroy_plan(&plan)) {
    puts("failed to destroy a plan");
    return 1;
  }
  return 0;
}

// compare rdft with naive implementation
static int test1 (
    const size_t nitems
) {
  double * const xs = malloc(nitems * sizeof(double));
  double complex * const ws = malloc(nitems * sizeof(double complex));
  double complex * const zs = malloc(nitems * sizeof(double complex));
  for (size_t i = 0; i < nitems; i++) {
    const double value = - 0.5 + 1. * rand() / RAND_MAX;
    xs[i] = value;
    ws[i] = value + 0. * I;
  }
  rdft_plan_t * plan = NULL;
  if (0 != rdft_init_plan(nitems, &plan)) {
    puts("failed to init a plan");
    return 1;
  }
  if (0 != rdft_exec_f(plan, xs)) {
    puts("forward execution failed");
    goto abort;
  }
  naive_dft(nitems, ws, zs);
  double dif = 0.;
  // real
  for (size_t i = 0; i < nitems / 2 + 1; i++) {
    dif += fabs(xs[i] - creal(zs[i]));
  }
  // imag
  for (size_t i = nitems / 2 + 1; i < nitems; i++) {
    dif += fabs(xs[i] - cimag(zs[i]));
  }
  dif /= 1. * nitems;
  printf("%zu % .15e\n", nitems, dif);
abort:
  free(xs);
  free(ws);
  free(zs);
  if (0 != rdft_destroy_plan(&plan)) {
    puts("failed to destroy a plan");
    return 1;
  }
  return 0;
}

// compare irdft with naive implementation
static int test2 (
    const size_t nitems
) {
  double * const xs = malloc(nitems * sizeof(double));
  double complex * const ws = malloc(nitems * sizeof(double complex));
  double complex * const zs = malloc(nitems * sizeof(double complex));
  for (size_t i = 0; i < nitems / 2 + 1; i++) {
    const double real = - 0.5 + 1. * rand() / RAND_MAX;
    const double imag = 0 == i || nitems / 2 == i ? 0. : - 0.5 + 1. * rand() / RAND_MAX;
    xs[i] = real;
    if (0 != i && nitems / 2 != i) {
      xs[nitems - i] = - imag;
    }
    ws[i] = real + imag * I;
  }
  for (size_t i = 1; i < nitems / 2; i++) {
    ws[nitems - i] = conj(ws[i]);
  }
  rdft_plan_t * plan = NULL;
  if (0 != rdft_init_plan(nitems, &plan)) {
    puts("failed to init a plan");
    return 1;
  }
  if (0 != rdft_exec_b(plan, xs)) {
    puts("backward execution failed");
    goto abort;
  }
  naive_idft(nitems, ws, zs);
  double dif = 0.;
  for (size_t i = 0; i < nitems; i++) {
    dif += fabs(creal(zs[i]) - xs[i]);
  }
  dif /= 1. * nitems;
  printf("%zu % .15e\n", nitems, dif);
abort:
  free(xs);
  free(ws);
  free(zs);
  if (0 != rdft_destroy_plan(&plan)) {
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
     2,  4,  8, 16,  32,  64, 128,  256,  512, 1024,
     6, 12, 24, 48,  96, 192, 384,  768, 1536, 3072,
    10, 20, 40, 80, 160, 320, 640, 1280, 2560, 5120,
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

