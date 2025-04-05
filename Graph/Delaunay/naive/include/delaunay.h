#if !defined(DELAUNAY_H)
#define DELAUNAY_H

#include <stddef.h>

typedef struct {
  size_t index;
  double x;
  double y;
} node_t;

typedef struct edge_t {
  const node_t * nodes[2];
  struct edge_t * next;
} edge_t;

extern int delaunay_triangulate(
    const size_t nnodes,
    const node_t * const nodes,
    edge_t ** const first_edge
);

#endif // DELAUNAY_H
