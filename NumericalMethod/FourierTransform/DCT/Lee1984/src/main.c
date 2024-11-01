#include <stdlib.h>
#include <math.h>
#include "dct.h"
#include "test.h"

static int test0 (
    const size_t nitems
) {
  // prepare
  double * buffers[] = {
    malloc(nitems * sizeof(double)),
    malloc(nitems * sizeof(double)),
  };
  // input signal
  for (size_t i = 0; i < nitems; i++) {
    buffers[0][i] = - 0.5 + 1. * rand() / RAND_MAX;
    buffers[1][i] = buffers[0][i];
  }
  // make a plan
  dct_plan_t * plan = NULL;
  if (0 != dct_init_plan(nitems, &plan)) {
    return 1;
  }
  // self check, forward followed by backward
  dct_exec_f(plan, buffers[0]);
  dct_exec_b(plan, buffers[0]);
  test_compare("DCT2 followed by DCT3", nitems, buffers[0], buffers[1]);
  // clean-up plan
  dct_destroy_plan(&plan);
  // clean-up buffers
  free(buffers[0]);
  free(buffers[1]);
  return 0;
}

static int test1 (
    const size_t nitems
) {
  // prepare
  double * buffers[] = {
    malloc(nitems * sizeof(double)),
    malloc(nitems * sizeof(double)),
    malloc(nitems * sizeof(double)),
  };
  // input signal
  for (size_t i = 0; i < nitems; i++) {
    buffers[0][i] = - 0.5 + 1. * rand() / RAND_MAX;
    buffers[1][i] = buffers[0][i];
  }
  // make a plan
  dct_plan_t * plan = NULL;
  if (0 != dct_init_plan(nitems, &plan)) {
    return 1;
  }
  // compare with naive DCT2
  test_exec_f(nitems, buffers[0], buffers[2]);
  dct_exec_f(plan, buffers[1]);
  test_compare("DCT2 compared with naive impl.", nitems, buffers[1], buffers[2]);
  // clean-up plan
  dct_destroy_plan(&plan);
  // clean-up buffers
  free(buffers[0]);
  free(buffers[1]);
  free(buffers[2]);
  return 0;
}

static int test2 (
    const size_t nitems
) {
  // prepare
  double * buffers[] = {
    malloc(nitems * sizeof(double)),
    malloc(nitems * sizeof(double)),
    malloc(nitems * sizeof(double)),
  };
  // input signal
  for (size_t i = 0; i < nitems; i++) {
    // for fair comparison, multiply by nitems to make the resulting values O(1)
    buffers[0][i] = (- 0.5 + 1. * rand() / RAND_MAX) * nitems;
    buffers[1][i] = buffers[0][i];
  }
  // make a plan
  dct_plan_t * plan = NULL;
  if (0 != dct_init_plan(nitems, &plan)) {
    return 1;
  }
  // compare with naive DCT3
  test_exec_b(nitems, buffers[0], buffers[2]);
  dct_exec_b(plan, buffers[1]);
  test_compare("DCT3 compared with naive impl.", nitems, buffers[1], buffers[2]);
  // clean-up plan
  dct_destroy_plan(&plan);
  // clean-up buffers
  free(buffers[0]);
  free(buffers[1]);
  free(buffers[2]);
  return 0;
}

int main (
    void
) {
  int retval = 0;
  srand(1 << 8);
  const size_t nitems_list[] = {
    1,  2,  4,  8, 16,  32,  64, 128,  256,  512, 1024,
    3,  6, 12, 24, 48,  96, 192, 384,  768, 1536, 3072,
    5, 10, 20, 40, 80, 160, 320, 640, 1280, 2560, 5120,
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

