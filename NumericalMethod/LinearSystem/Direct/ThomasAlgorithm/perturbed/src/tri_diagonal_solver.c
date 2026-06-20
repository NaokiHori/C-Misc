#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <float.h>
#include "tri_diagonal_solver.h"

struct tri_diagonal_solver_internal_t {
  // auxiliary buffers
  double * v;
  double * w;
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
    void * const ptr
) {
  free(ptr);
}

int tri_diagonal_solver_initialise (
    const size_t size,
    tri_diagonal_solver_t ** const tri_diagonal_solver
){
  const size_t minimum_size = 3;
  if (size < minimum_size) {
    fprintf(stderr, "size %zu is too small, give larger than %zu\n", size, minimum_size);
    return 1;
  }
  *tri_diagonal_solver = memory_alloc(1, sizeof(tri_diagonal_solver_t));
  (*tri_diagonal_solver)->internal = memory_alloc(1, sizeof(tri_diagonal_solver_internal_t));
  (*tri_diagonal_solver)->size = size;
  (*tri_diagonal_solver)->l = memory_alloc(size, sizeof(double));
  (*tri_diagonal_solver)->c = memory_alloc(size, sizeof(double));
  (*tri_diagonal_solver)->u = memory_alloc(size, sizeof(double));
  (*tri_diagonal_solver)->q = memory_alloc(size, sizeof(double));
  (*tri_diagonal_solver)->internal->v = memory_alloc(size, sizeof(double));
  (*tri_diagonal_solver)->internal->w = memory_alloc(size - 1, sizeof(double));
  return 0;
}

int tri_diagonal_solver_solve (
    tri_diagonal_solver_t * const tri_diagonal_solver
) {
  if (NULL == tri_diagonal_solver) {
    return 1;
  }
  const size_t size = tri_diagonal_solver->size;
  const double * const l = tri_diagonal_solver->l;
  const double * const c = tri_diagonal_solver->c;
  const double * const u = tri_diagonal_solver->u;
  double * const v = tri_diagonal_solver->internal->v;
  double * const w = tri_diagonal_solver->internal->w;
  double * const q = tri_diagonal_solver->q;
  // prepare a sub system | 6
  for (size_t i = 0; i < size - 1; i++) {
    w[i] =
        i ==        0 ? - l[       0]
      : i == size - 2 ? - u[size - 2]
      : 0.;
  }
  // divide the first row by center-diagonal term | 3
  v[0] = u[0] / c[0];
  q[0] = q[0] / c[0];
  w[0] = w[0] / c[0];
  // forward sweep all rows | 8
  for (size_t i = 1; i < size - 2; i++) {
    // assume positive-definite system
    //   to skip zero-division checks
    const double val = 1. / (c[i] - l[i] * v[i - 1]);
    v[i] = val *  u[i];
    q[i] = val * (q[i] - l[i] * q[i - 1]);
    w[i] = val * (w[i] - l[i] * w[i - 1]);
  }
  // forward sweep the last row | 10
  // NOTE: do the same thing but consider singularity (degeneracy)
  const double val = c[size - 2] - l[size - 2] * v[size - 3];
  if (DBL_EPSILON < fabs(val)) {
    q[size - 2] = 1. / val * (q[size - 2] - l[size - 2] * q[size - 3]);
    w[size - 2] = 1. / val * (w[size - 2] - l[size - 2] * w[size - 3]);
  } else {
    // singular
    q[size - 2] = 0.;
    w[size - 2] = 0.;
  }
  // backward substitution | 7
  for (size_t i = size - 3; ; i--) {
    q[i] -= v[i] * q[i + 1];
    w[i] -= v[i] * w[i + 1];
    if (0 == i) {
      break;
    }
  }
  // couple two systems to find the answer | 6
  const double num = q[size - 1] - u[size - 1] * q[0] - l[size - 1] * q[size - 2];
  const double den = c[size - 1] + u[size - 1] * w[0] + l[size - 1] * w[size - 2];
  q[size - 1] = fabs(den) < DBL_EPSILON ? 0. : num / den;
  for (size_t i = 0; i < size - 1; i++) {
    q[i] = q[i] + q[size - 1] * w[i];
  }
  return 0;
}

int tri_diagonal_solver_finalise (
    tri_diagonal_solver_t ** const tri_diagonal_solver
){
  memory_free((*tri_diagonal_solver)->l);
  memory_free((*tri_diagonal_solver)->c);
  memory_free((*tri_diagonal_solver)->u);
  memory_free((*tri_diagonal_solver)->q);
  memory_free((*tri_diagonal_solver)->internal->v);
  memory_free((*tri_diagonal_solver)->internal->w);
  memory_free((*tri_diagonal_solver)->internal);
  memory_free(*tri_diagonal_solver);
  *tri_diagonal_solver = NULL;
  return 0;
}

