#if !defined(MATRIX_H)
#define MATRIX_H

#include <stddef.h>
#include <stdint.h>

typedef struct {
  int64_t numerator;
  int64_t denominator;
} rational_t;

extern rational_t add(
  const rational_t * const a,
  const rational_t * const b
);

extern rational_t subtract(
  const rational_t * const a,
  const rational_t * const b
);

extern rational_t multiply(
  const rational_t * const a,
  const rational_t * const b
);

extern int compute_inverse_matrix(
  const size_t nitems,
  rational_t * const matrix_a,
  rational_t * const matrix_b
);

#endif // MATRIX_H
