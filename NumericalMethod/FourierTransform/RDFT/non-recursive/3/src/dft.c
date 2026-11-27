#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include "dft.h"

static const double pi = 3.14159265358979324;

struct dft_plan_t {
  size_t nitems;
};

static void * memory_alloc(
    const size_t size
) {
  void * const ptr = malloc(size);
  if (NULL == ptr) {
    fprintf(
        stderr,
        "[FATAL %s:%d] failed to allocate %zu bytes\n",
        __FILE__,
        __LINE__,
        size
    );
    exit(EXIT_FAILURE);
  }
  return ptr;
}

static void memory_free(
    void * const ptr
) {
  free(ptr);
}

static int transform(
    const dft_plan_t * const plan,
    const double sign,
    double * const restrict reals,
    double * const restrict imags
) {
  const size_t nitems = plan->nitems;
  double * const tmp_reals = memory_alloc(nitems * sizeof(double));
  double * const tmp_imags = memory_alloc(nitems * sizeof(double));
  for (size_t i = 0; i < nitems; i++) {
    double * const tmp_real = tmp_reals + i;
    double * const tmp_imag = tmp_imags + i;
    *tmp_real = 0.;
    *tmp_imag = 0.;
    for (size_t j = 0; j < nitems; j++) {
      const double real = reals[j];
      const double imag = imags[j];
      const double phase = sign * 2. * pi * i * j / nitems;
      const double twiddle_cos = cos(phase);
      const double twiddle_sin = sin(phase);
      *tmp_real += real * twiddle_cos - imag * twiddle_sin;
      *tmp_imag += real * twiddle_sin + imag * twiddle_cos;
    }
  }
  for (size_t i = 0; i < nitems; i++) {
    reals[i] = tmp_reals[i];
    imags[i] = tmp_imags[i];
  }
  memory_free(tmp_reals);
  memory_free(tmp_imags);
  return 0;
}

static int scale(
    const size_t nitems,
    const double prefactor,
    double * const values
) {
  for (size_t i = 0; i < nitems; i++) {
    values[i] *= prefactor;
  }
  return 0;
}

int dft_execute_inplace_forward_transform(
    const dft_plan_t * const plan,
    double * const restrict reals,
    double * const restrict imags
) {
  transform(plan, -1., reals, imags);
  return 0;
}

int dft_execute_inplace_backward_transform(
    const dft_plan_t * const plan,
    double * const restrict reals,
    double * const restrict imags
) {
  transform(plan, 1., reals, imags);
  const size_t nitems = plan->nitems;
  scale(nitems, 1. / nitems, reals);
  scale(nitems, 1. / nitems, imags);
  return 0;
}

int dft_init_plan(
    const size_t nitems,
    dft_plan_t ** const plan
) {
  if (nitems < 1) {
    fprintf(stderr, "nitems should be at least 1: %zu\n", nitems);
    *plan = NULL;
    return 1;
  }
  *plan = memory_alloc(1 * sizeof(dft_plan_t));
  (*plan)->nitems = nitems;
  puts("NOTE: stub dft O(N^2) is used for convenience");
  return 0;
}

int dft_destroy_plan(
    dft_plan_t ** const plan
) {
  if (NULL == *plan) {
    return 0;
  }
  memory_free(*plan);
  *plan = NULL;
  return 0;
}

