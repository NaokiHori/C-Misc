#if !defined(COMMON_H)
#define COMMON_H

#include <stdbool.h>
#include <stddef.h>

// number of nodes for each element: should be fixed to 2
#define N_ELEMENT_NODES 2
#define NDIMS 2

typedef struct {
  size_t index;
  bool is_fixed[NDIMS];
  double position[NDIMS];
} node_t;

typedef struct {
  double length;
  const node_t * nodes[N_ELEMENT_NODES];
} element_t;

#endif // COMMON_H
