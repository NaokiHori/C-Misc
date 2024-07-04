#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <float.h>
#include "mysolver.h"

struct mysolver_internal_t {
  // auxiliary buffer
  double * v;
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
  double * restrict q = mysolver->q;
  // divide the first row by center-diagonal term
  v[0] = u[0] / c[0];
  q[0] = q[0] / c[0];
  // forward sweep
  for (size_t i = 1; i < size - 1; i++) {
    // assume positive-definite system
    //   to skip zero-division checks
    const double val = 1. / (c[i] - l[i] * v[i - 1]);
    v[i] = val * u[i];
    q[i] = val * (q[i] - l[i] * q[i - 1]);
  }
  // last row, do the same thing but consider singularity (degeneracy)
  const double val = c[size - 1] - l[size - 1] * v[size - 2];
  if (DBL_EPSILON < fabs(val)) {
    q[size - 1] = 1. / val * (q[size - 1] - l[size - 1] * q[size - 2]);
  } else {
    // singular, enforce zero mean
    q[size - 1] = 0.;
  }
  // backward substitution
  for (size_t i = size - 2; ; i--) {
    q[i] -= v[i] * q[i + 1];
    if (0 == i) {
      break;
    }
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
  memory_free((*mysolver)->internal);
  memory_free(*mysolver);
  *mysolver = NULL;
  return 0;
}

