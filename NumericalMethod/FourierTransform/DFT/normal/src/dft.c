#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <complex.h>
#include "dft.h"

static const double pi = 3.14159265358979324;

struct dft_plan_t {
  // length of real signal
  // NOTE: for inverse transform, the input complex signal
  //       should have nitems / 2 + 1 elements
  size_t nitems;
  // pre-computed cosine / sine values
  double * table_cos;
  double * table_sin;
  // internal buffer
  double complex * buf;
};

static void * memory_alloc (
    const size_t size
) {
  void * const ptr = malloc(size);
  if (NULL == ptr) {
    fprintf(stderr, "[FATAL %s:%d] failed to allocate %zu bytes\n", __FILE__, __LINE__, size);
    return NULL;
  }
  return ptr;
}

static void memory_free (
    void * const ptr
) {
  free(ptr);
}

// recursive Cooley-Tukey FFT for complex input/output
static int dft (
    const size_t nitems,
    const double sign,
    const size_t stride,
    const double * const table_cos,
    const double * const table_sin,
    const double complex * xs,
    double complex * ys
) {
  if (1 == nitems) {
    ys[0] = xs[0];
  } else if (0 == nitems % 2) {
    dft(nitems / 2, sign, stride * 2, table_cos, table_sin, xs         , ys             );
    dft(nitems / 2, sign, stride * 2, table_cos, table_sin, xs + stride, ys + nitems / 2);
    for (size_t i = 0; i < nitems / 2; i++) {
      const size_t j = i + nitems / 2;
      const double c = table_cos[stride * i];
      const double s = table_sin[stride * i];
      const double complex twiddle = c + sign * I * s;
      const double complex e = ys[i];
      const double complex o = ys[j] * twiddle;
      ys[i] = e + o;
      ys[j] = e - o;
    }
  } else {
    // naive O(N^2) DFT
    for (size_t k = 0; k < nitems; k++) {
      double complex * const y = ys + k;
      *y = 0. + I * 0.;
      for (size_t n = 0; n < nitems; n++) {
        *y += xs[stride * n] * cexp(sign * 2. * pi * n * k * I / nitems);
      }
    }
  }
	return 0;
}

int dft_exec_f (
    dft_plan_t * const plan,
    double complex * const xs
) {
  if (NULL == plan) {
    puts("uninitialized plan is passed");
    return 1;
  }
  const size_t nitems = plan->nitems;
  const double * const table_cos = plan->table_cos;
  const double * const table_sin = plan->table_sin;
  double complex * const ys = plan->buf;
  dft(nitems, - 1., 1, table_cos, table_sin, xs, ys);
  for (size_t i = 0; i < nitems; i++) {
    xs[i] = ys[i];
  }
  return 0;
}

int dft_exec_b (
    dft_plan_t * const plan,
    double complex * const xs
) {
  if (NULL == plan) {
    puts("uninitialized plan is passed");
    return 1;
  }
  const size_t nitems = plan->nitems;
  const double * const table_cos = plan->table_cos;
  const double * const table_sin = plan->table_sin;
  double complex * const ys = plan->buf;
  dft(nitems, + 1., 1, table_cos, table_sin, xs, ys);
  for (size_t i = 0; i < nitems; i++) {
    xs[i] = ys[i];
  }
  return 0;
}

int dft_init_plan (
    const size_t nitems,
    dft_plan_t ** const plan
) {
  *plan = memory_alloc(1 * sizeof(dft_plan_t));
  (*plan)->nitems = nitems;
  double ** const table_cos = &(*plan)->table_cos;
  double ** const table_sin = &(*plan)->table_sin;
  double complex ** const buf = &(*plan)->buf;
  *table_cos = memory_alloc(nitems * sizeof(double));
  *table_sin = memory_alloc(nitems * sizeof(double));
  *buf = memory_alloc(nitems * sizeof(double complex));
  // prepare cosine / sine tables
  for (size_t i = 0; i < nitems; i++) {
    (*table_cos)[i] = cos(2. * pi * i / nitems);
    (*table_sin)[i] = sin(2. * pi * i / nitems);
  }
  return 0;
}

// clean-up a plan
int dft_destroy_plan (
    dft_plan_t ** const plan
) {
  memory_free((*plan)->table_cos);
  memory_free((*plan)->table_sin);
  memory_free((*plan)->buf);
  memory_free(*plan);
  *plan = NULL;
  return 0;
}

