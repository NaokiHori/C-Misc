#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "dct.h"

static const double pi = 3.141592653589793;
static const double sqrt2 = 1.4142135623730951;

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

static int dct2 (
    const size_t nitems,
    const size_t stride,
    const double * const restrict table,
    double * const restrict xs,
    double * const restrict ys
) {
  if (1 == nitems) {
    xs[0] *= 2.;
  } else if (2 == nitems) {
		const double v0 = xs[0];
		const double v1 = xs[1];
		xs[0] = 2.    * v0 + 2.    * v1;
		xs[1] = sqrt2 * v0 - sqrt2 * v1;
  } else if (0 == nitems % 2) {
    // divide and conquer, forward | 22
    const size_t nhalfs = nitems / 2;
    for (size_t n = 0; n < nhalfs; n++) {
      // c: inverse of cos(beta)
      const double c = table[(2 * n + 1) * stride];
      const double v0 = xs[             n];
      const double v1 = xs[nitems - 1 - n];
      ys[n         ] = 1. * (v0 + v1);
      ys[n + nhalfs] = c  * (v0 - v1);
    }
    // solve two sub problems
    dct2(nhalfs, stride * 2, table, ys +      0, xs);
    dct2(nhalfs, stride * 2, table, ys + nhalfs, xs);
    // even frequencies
    for (size_t k = 0; k < nhalfs; k++) {
      xs[k * 2 + 0] = ys[k];
    }
    // odd frequencies
    for (size_t k = 0; k < nhalfs - 1; k++) {
      xs[k * 2 + 1] = ys[nhalfs + k] + ys[nhalfs + k + 1];
    }
    // for the last element, "k+1"-th element is zero
    xs[nitems - 1] = ys[nitems - 1];
  } else {
    // fallback to N^2 DCT2
    for (size_t k = 0; k < nitems; k++) {
      double * y = ys + k;
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

static int dct3 (
    const size_t nitems,
    const size_t stride,
    const double * const restrict table,
    double * const restrict xs,
    double * const restrict ys
) {
  if (1 == nitems) {
    xs[0] *= 2.;
  } else if (2 == nitems) {
    const double v0 = xs[0];
    const double v1 = xs[1];
    xs[0] = 2. * v0 + sqrt2 * v1;
    xs[1] = 2. * v0 - sqrt2 * v1;
  } else if (0 == nitems % 2) {
    // divide and conquer, backward | 22
    const size_t nhalfs = nitems / 2;
    // first  idct: ys[     0 : nhalfs - 1], even elements
    // second idct: ys[nhalfs : nitems - 1], sum of two odd elements
    for (size_t k = 0; k < nhalfs; k++) {
      ys[k] = xs[k * 2];
    }
    ys[nhalfs] = xs[1];
    for (size_t k = 1; k < nhalfs; k++) {
      ys[nhalfs + k] = xs[k * 2 - 1] + xs[k * 2 + 1];
    }
    // solve two sub problems
    dct3(nhalfs, stride * 2, table, ys         , xs);
    dct3(nhalfs, stride * 2, table, ys + nhalfs, xs);
    // combine results of sub problems
    for (size_t n = 0; n < nhalfs; n++) {
      // c: inverse of cos(beta)
      const double c = table[(2 * n + 1) * stride];
      const double v0 = 1. * ys[         n];
      const double v1 = c  * ys[nhalfs + n];
      xs[             n] = v0 + v1;
      xs[nitems - 1 - n] = v0 - v1;
    }
  } else {
    // fallback to N^2 DCT3
    for (size_t n = 0; n < nitems; n++) {
      double * y = ys + n;
      *y = 0.;
      for (size_t k = 0; k < nitems; k++) {
        const double phase = pi * (2 * n + 1) * k / (2 * nitems);
        *y += 2. * xs[k] * cos(phase);
      }
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
  // normalize 0-th wave number before executing DCT3 | 1
  xs[0] *= 0.5;
  dct3(nitems, 1, table, xs, ys);
  return 0;
}

