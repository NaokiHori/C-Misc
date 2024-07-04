#if !defined(DIJKSTRA_H)
#define DIJKSTRA_H

#include <stddef.h>
#include <float.h>

#define DIJKSTRA_INFTY_COST DBL_MAX;

typedef struct {
  size_t node0;
  size_t node1;
  double cost;
} link_t;

extern int dijkstra (
    const size_t nnodes,
    const size_t nlinks,
    const link_t * links
);

#endif // DIJKSTRA_H
