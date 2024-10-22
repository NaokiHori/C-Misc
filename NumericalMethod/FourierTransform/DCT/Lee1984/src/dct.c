#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "dct.h"

static const double pi = 3.141592653589793;

struct dct_plan_t {
  // size of the input / output signals
  size_t nitems;
  // trigonometric table
  //   1 / (2 cos( (pi i) / (2 N) ))
  //   where i = 0, 1, ..., N - 1
  double * table;
  // internal buffer
  double * buf;
};

static void * memory_alloc (
    const size_t size
) {
  void * const ptr = malloc(size);
  if (NULL == ptr) {
    fprintf(stderr, "[dct FATAL] failed to allocate %zu bytes\n", size);
    return NULL;
  }
  return ptr;
}

static void memory_free (
    void * const ptr
) {
  free(ptr);
}

// forward transform | 54
static int dct2 (
    const size_t nitems,
    const size_t stride,
    const double * const restrict table,
    double * const restrict xs,
    double * const restrict ys
) {
  if (1 == nitems) {
    xs[0] *= 2.;
    return 0;
  }
  if (0 == nitems % 2) {
    // divide and conquer
    const size_t nhalfs = nitems / 2;
    double * const buf0 = ys +      0;
    double * const buf1 = ys + nhalfs;
    // create input buffers of DCT-II
    for (size_t n = 0; n < nhalfs; n++) {
      // c: 1 / cos(beta)
      const double c = table[(2 * n + 1) * stride];
      const double value0 = xs[             n];
      const double value1 = xs[nitems - 1 - n];
      buf0[n] = 1. * (value0 + value1);
      buf1[n] = c  * (value0 - value1);
    }
    // solve two sub problems
    dct2(nhalfs, stride * 2, table, buf0, xs);
    dct2(nhalfs, stride * 2, table, buf1, xs);
    // distribute results
    for (size_t k = 0; k < nhalfs; k++) {
      // to even frequencies
      xs[k * 2 + 0] = buf0[k];
      // to odd frequencies
      // for the last element, "k+1"-th element is zero
      const double value0 =                        buf1[k    ];
      const double value1 = nhalfs - 1 == k ? 0. : buf1[k + 1];
      xs[k * 2 + 1] = value0 + value1;
    }
  } else {
    // fallback to N^2 DCT2
    for (size_t k = 0; k < nitems; k++) {
      double * const y = ys + k;
      *y = 0.;
      for (size_t n = 0; n < nitems; n++) {
        const double phase = pi * (2 * n + 1) * k / (2 * nitems);
        *y += 2. * xs[n] * cos(phase);
      }
    }
    for (size_t k = 0; k < nitems; k++) {
      xs[k] = ys[k];
    }
  }
  return 0;
}

// backward transform | 51
static int dct3 (
    const size_t nitems,
    const size_t stride,
    const double * const restrict table,
    double * const restrict xs,
    double * const restrict ys
) {
  if (1 == nitems) {
    return 0;
  }
  if (0 == nitems % 2) {
    // divide and conquer
    const size_t nhalfs = nitems / 2;
    double * const buf0 = ys +      0;
    double * const buf1 = ys + nhalfs;
    // create input buffers of DCT-III
    for (size_t k = 0; k < nhalfs; k++) {
      buf0[k] = xs[k * 2];
      const double value0 = 0 == k ? 0. : xs[k * 2 - 1];
      const double value1 =               xs[k * 2 + 1];
      buf1[k] = value0 + value1;
    }
    // solve two sub problems
    dct3(nhalfs, stride * 2, table, buf0, xs);
    dct3(nhalfs, stride * 2, table, buf1, xs);
    // combine results of sub problems
    for (size_t n = 0; n < nhalfs; n++) {
      // c: 1 / cos(beta)
      const double c = table[(2 * n + 1) * stride];
      const double value0 = 0.5 * buf0[n];
      const double value1 = 0.5 * c * buf1[n];
      xs[             n] = value0 + value1;
      xs[nitems - 1 - n] = value0 - value1;
    }
  } else {
    // fallback to N^2 DCT3
    for (size_t n = 0; n < nitems; n++) {
      double * const y = ys + n;
      *y = 0.;
      for (size_t k = 0; k < nitems; k++) {
        const double phase = pi * (2 * n + 1) * k / (2 * nitems);
        *y += xs[k] * cos(phase);
      }
      *y /= 1. * nitems;
    }
    for (size_t n = 0; n < nitems; n++) {
      xs[n] = ys[n];
    }
  }
  return 0;
}

int dct_init_plan (
    const size_t nitems,
    dct_plan_t ** const plan
) {
  // allocate, initalise, and pack
  *plan = memory_alloc(1 * sizeof(dct_plan_t));
  if (NULL == *plan) {
    return 1;
  }
  (*plan)->nitems = nitems;
  // create table
  double ** table = &(*plan)->table;
  *table = memory_alloc(nitems * sizeof(double));
  if (NULL == *table) {
    return 1;
  }
  for (size_t i = 0; i < nitems; i++) {
    const double phase = (pi * i) / (2 * nitems);
    (*table)[i] = 0.5 / cos(phase);
  }
  // prepare internal buffer
  double ** buf = &(*plan)->buf;
  *buf = memory_alloc(nitems * sizeof(double));
  if (NULL == *buf) {
    return 1;
  }
  return 0;
}

int dct_destroy_plan (
    dct_plan_t ** const plan
) {
  if (NULL == *plan) {
    fprintf(stderr, "the plan is NULL\n");
    return 1;
  }
  memory_free((*plan)->table);
  memory_free((*plan)->buf);
  memory_free(*plan);
  *plan = NULL;
  return 0;
}

int dct_exec_f (
    dct_plan_t * const plan,
    double * const xs
){
  if (NULL == plan) {
    fprintf(stderr, "the plan is NULL\n");
    return 1;
  }
  const size_t nitems = plan->nitems;
  const double * const restrict table = plan->table;
  double * const restrict ys = plan->buf;
  dct2(nitems, 1, table, xs, ys);
  return 0;
}

int dct_exec_b (
    dct_plan_t * const plan,
    double * const xs
) {
  if (NULL == plan) {
    fprintf(stderr, "the plan is NULL\n");
    return 1;
  }
  const size_t nitems = plan->nitems;
  const double * const restrict table = plan->table;
  double * const restrict ys = plan->buf;
  // normalize 0-th wave number before executing DCT3 | 2
  xs[0] *= 0.5;
  dct3(nitems, 1, table, xs, ys);
  return 0;
}

