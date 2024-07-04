#include <stdlib.h>
#include <math.h>
#include "mydct.h"
#include "test.h"

int main (
    void
) {
  srand(1 << 8);
  const size_t nitems = 3 * (1 << 8);
  // prepare
  double * x0s = malloc(nitems * sizeof(double));
  double * x1s = malloc(nitems * sizeof(double));
  double * y0s = malloc(nitems * sizeof(double));
  double * y1s = malloc(nitems * sizeof(double));
  // input signal
  for (size_t i = 0; i < nitems; i++) {
    x0s[i] = - 0.5 + 1. * rand() / RAND_MAX;
  }
  // make a plan
  mydct_plan_t * plan = NULL;
  if (0 != mydct_init_plan(nitems, true, &plan)) {
    return 1;
  }
  // self check, forward followed by backward
  // x0s and x1s should be identical, up to rounding error
  mydct_exec_f(plan, x0s, y0s);
  mydct_exec_b(plan, y0s, x1s);
  for (size_t i = 0; i < nitems; i++) {
    x1s[i] /= 2. * nitems;
  }
  test_compare("DCT2 followed by DCT3", nitems, x0s, x1s);
  // compare with naive DCT2
  test_exec_f(nitems, x0s, y0s);
  mydct_exec_f(plan, x0s, y1s);
  test_compare("DCT2 compared with naive impl.", nitems, y0s, y1s);
  // compare with naive DCT3
  test_exec_b(nitems, x0s, y0s);
  mydct_exec_b(plan, x0s, y1s);
  test_compare("DCT3 compared with naive impl.", nitems, y0s, y1s);
  // clean-up plan
  mydct_destroy_plan(&plan);
  // clean-up buffers
  free(x0s);
  free(x1s);
  free(y0s);
  free(y1s);
  return 0;
}

