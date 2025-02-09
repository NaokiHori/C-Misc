#if !defined(TYPES_H)
#define TYPES_H

#include <stdbool.h> // bool
#include "vector.h" // vector_t

typedef struct {
  vector_t vector;
  double label;
  bool is_support_vector;
} dataset_t;

#endif // TYPES_H
