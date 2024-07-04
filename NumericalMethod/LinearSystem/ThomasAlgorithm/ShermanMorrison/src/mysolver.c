#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <float.h>
#include "mysolver.h"

struct mysolver_internal_t {
  // auxiliary buffers
  double * restrict v;
  double * restrict w;
};

static void * memory_alloc (
    const size_t count,
    const size_t size
) {
  void * const ptr = malloc(count * size);
  if (NULL == ptr) {
    fprintf(stderr, "memory allocation error: (%zu, %zu)\n", count, size);
    exit(EXIT_FAILURE);
  }
  return ptr;
}

static void memory_free (
    void * ptr
) {
  free(ptr);
}

int mysolver_initialise (
    const size_t size,
    mysolver_t ** mysolver
){
  const size_t minimum_size = 3;
  if (size < minimum_size) {
    fprintf(stderr, "size %zu is too small, give larger than %zu\n", size, minimum_size);
    return 1;
  }
  *mysolver = memory_alloc(1, sizeof(mysolver_t));
  (*mysolver)->internal = memory_alloc(1, sizeof(mysolver_internal_t));
  (*mysolver)->size = size;
  (*mysolver)->l = memory_alloc(size, sizeof(double));
  (*mysolver)->c = memory_alloc(size, sizeof(double));
  (*mysolver)->u = memory_alloc(size, sizeof(double));
  (*mysolver)->q = memory_alloc(size, sizeof(double));
  (*mysolver)->internal->v = memory_alloc(size, sizeof(double));
  (*mysolver)->internal->w = memory_alloc(size - 1, sizeof(double));
  return 0;
}

int mysolver_solve (
    mysolver_t * mysolver
) {
  if (NULL == mysolver) {
    return 1;
  }
  const size_t size = mysolver->size;
  const double * restrict l = mysolver->l;
  const double * restrict c = mysolver->c;
  const double * restrict u = mysolver->u;
  double * restrict v = mysolver->internal->v;
  double * restrict w = mysolver->internal->w;
  double * restrict q = mysolver->q;
  // consider a perturbed system as well
  for (size_t i = 0; i < size - 1; i++) {
    w[i]
      = i ==        0 ? -l[i]
      : i == size - 2 ? -u[i]
      : 0.;
  }
  // divide the first row by center-diagonal term
  v[0] = u[0] / c[0];
  q[0] = q[0] / c[0];
  w[0] = w[0] / c[0];
  // forward sweep
  for (size_t i = 1; i < size - 2; i++) {
    // assume positive-definite system
    //   to skip zero-division checks
    const double val = 1. / (c[i] - l[i] * v[i-1]);
    v[i] = val * u[i];
    q[i] = val * (q[i] - l[i] * q[i-1]);
    w[i] = val * (w[i] - l[i] * w[i-1]);
  }
  // last row, do the same thing but consider singularity (degeneracy)
  const double val = c[size - 2] - l[size - 2] * v[size - 3];
  if (DBL_EPSILON < fabs(val)) {
    q[size - 2] = 1. / val * (q[size - 2] - l[size - 2] * q[size - 3]);
    w[size - 2] = 1. / val * (w[size - 2] - l[size - 2] * w[size - 3]);
  } else {
    // singular, enforce zero mean
    q[size - 2] = 0.;
    w[size - 2] = 0.;
  }
  // backward substitution
  for (size_t i = size - 3; ; i--) {
    q[i] -= v[i] * q[i+1];
    w[i] -= v[i] * w[i+1];
    if (0 == i) {
      break;
    }
  }
  // couple two systems to find the answer
  const double num = q[size - 1] - u[size - 1] * q[0] - l[size - 1] * q[size - 2];
  const double den = c[size - 1] + u[size - 1] * w[0] + l[size - 1] * w[size - 2];
  q[size - 1] = fabs(den) < DBL_EPSILON ? 0. : num / den;
  for (size_t i = 0; i < size - 1; i++) {
    q[i] = q[i] + q[size - 1] * w[i];
  }
  return 0;
}

int mysolver_finalise (
    mysolver_t ** mysolver
){
  memory_free((*mysolver)->l);
  memory_free((*mysolver)->c);
  memory_free((*mysolver)->u);
  memory_free((*mysolver)->q);
  memory_free((*mysolver)->internal->v);
  memory_free((*mysolver)->internal->w);
  memory_free((*mysolver)->internal);
  memory_free(*mysolver);
  *mysolver = NULL;
  return 0;
}

