#include <complex.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include "dft.h"

static int naive_dft(
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

static int naive_idft(
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
static int test0(
    const size_t nitems
) {
  double complex * const answers = malloc(nitems * sizeof(double complex));
  double * const reals = malloc(nitems * sizeof(double));
  double * const imags = malloc(nitems * sizeof(double));
  for (size_t i = 0; i < nitems; i++) {
    const double real = - 0.5 + 1. * rand() / RAND_MAX;
    const double imag = - 0.5 + 1. * rand() / RAND_MAX;
    answers[i] = real + I * imag;
    reals[i] = real;
    imags[i] = imag;
  }
  dft_plan_t * plan = NULL;
  if (0 != dft_init_plan(nitems, &plan)) {
    puts("failed to init a plan");
    return 1;
  }
  if (0 != dft_execute_inplace_forward_transform(plan, reals, imags)) {
    puts("forward execution failed");
    goto abort;
  }
  if (0 != dft_execute_inplace_backward_transform(plan, reals, imags)) {
    puts("backward execution failed");
    goto abort;
  }
  double dif = 0.;
  for (size_t i = 0; i < nitems; i++) {
    dif += cabs((reals[i] + I * imags[i]) - answers[i]);
  }
  dif /= nitems;
  printf("DFT followed by iDFT: %zu % .15e\n", nitems, dif);
abort:
  free(answers);
  free(reals);
  free(imags);
  if (0 != dft_destroy_plan(&plan)) {
    puts("failed to destroy a plan");
    return 1;
  }
  return 0;
}

// compare dft with naive implementation
static int test1(
    const size_t nitems
) {
  double complex * const answers = malloc(nitems * sizeof(double complex));
  double * const reals = malloc(nitems * sizeof(double));
  double * const imags = malloc(nitems * sizeof(double));
  for (size_t i = 0; i < nitems; i++) {
    const double real = - 0.5 + 1. * rand() / RAND_MAX;
    const double imag = - 0.5 + 1. * rand() / RAND_MAX;
    answers[i] = real + I * imag;
    reals[i] = real;
    imags[i] = imag;
  }
  dft_plan_t * plan = NULL;
  if (0 != dft_init_plan(nitems, &plan)) {
    puts("failed to init a plan");
    return 1;
  }
  if (0 != dft_execute_inplace_forward_transform(plan, reals, imags)) {
    puts("forward execution failed");
    goto abort;
  }
  naive_dft(nitems, answers);
  double dif = 0.;
  for (size_t i = 0; i < nitems; i++) {
    dif += cabs((reals[i] + I * imags[i]) - answers[i]);
  }
  dif /= 1. * nitems;
  printf("DFT compared with naive impl.: %zu % .15e\n", nitems, dif);
abort:
  free(answers);
  free(reals);
  free(imags);
  if (0 != dft_destroy_plan(&plan)) {
    puts("failed to destroy a plan");
    return 1;
  }
  return 0;
}

// compare idft with naive implementation
static int test2(
    const size_t nitems
) {
  double complex * const answers = malloc(nitems * sizeof(double complex));
  double * const reals = malloc(nitems * sizeof(double));
  double * const imags = malloc(nitems * sizeof(double));
  for (size_t i = 0; i < nitems; i++) {
    const double real = (- 0.5 + 1. * rand() / RAND_MAX) * nitems;
    const double imag = (- 0.5 + 1. * rand() / RAND_MAX) * nitems;
    answers[i] = real + I * imag;
    reals[i] = real;
    imags[i] = imag;
  }
  dft_plan_t * plan = NULL;
  if (0 != dft_init_plan(nitems, &plan)) {
    puts("failed to init a plan");
    return 1;
  }
  if (0 != dft_execute_inplace_backward_transform(plan, reals, imags)) {
    puts("backward execution failed");
    goto abort;
  }
  naive_idft(nitems, answers);
  double dif = 0.;
  for (size_t i = 0; i < nitems; i++) {
    dif += cabs((reals[i] + I * imags[i]) - answers[i]);
  }
  dif /= 1. * nitems;
  printf("iDFT compared with naive impl.: %zu % .15e\n", nitems, dif);
abort:
  free(answers);
  free(reals);
  free(imags);
  if (0 != dft_destroy_plan(&plan)) {
    puts("failed to destroy a plan");
    return 1;
  }
  return 0;
}

int main(
    void
) {
  int retval = 0;
  const size_t nitems_list[] = {
    1, 2, 3, 4, 5, 6, 7, 8, 9, 10,
    11, 12, 13, 14, 15, 16, 18, 20, 21,
    22, 24, 25, 26, 27, 28, 30, 32, 36,
    48, 64, 96, 128, 192, 256, 384, 512, 768, 1024,
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

