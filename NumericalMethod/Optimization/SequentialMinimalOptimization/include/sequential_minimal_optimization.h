#if !defined(SEQUENTIAL_MINIMAL_OPTIMIZATION_H)
#define SEQUENTIAL_MINIMAL_OPTIMIZATION_H

#include <stddef.h>

#define NDIMS 2

typedef enum {
  NEGATIVE = -1,
  POSITIVE = 1,
} category_t;

typedef struct {
  double at[NDIMS];
  category_t category;
} sample_t;

typedef struct {
  size_t nitems;
  sample_t * samples;
  double * multipliers;
  size_t n_support_vectors;
  size_t * support_vector_indices;
  double * kernel;
  double (* compute_kernel)(
      const double (* const at0)[NDIMS],
      const double (* const at1)[NDIMS]
  );
} categorizer_t;

extern int categorizer_initialize(
    const size_t nitems,
    double (* const compute_kernel)(
        const double (* const at0)[NDIMS],
        const double (* const at1)[NDIMS]
    ),
    categorizer_t * const categorizer
);

extern int categorizer_finalize(
    categorizer_t * const categorizer
);

extern int categorizer_optimize(
    categorizer_t * const categorizer,
    double * const residual
);

extern int categorizer_categorize(
    const categorizer_t * const categorizer,
    const double (* const at)[NDIMS],
    category_t * const category
);

#endif // SEQUENTIAL_MINIMAL_OPTIMIZATION_H
