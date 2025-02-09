#include <stddef.h>
#include "types.h"

static double inner_product(
    const vector_t * const a,
    const vector_t * const b
) {
  double result = 0.;
  for (size_t dim = 0; dim < NDIMS + 1; dim++) {
    result += (*a)[dim] * (*b)[dim];
  }
  return result;
}

const vector_op_t vector_op = {
  .inner_product = inner_product,
};

