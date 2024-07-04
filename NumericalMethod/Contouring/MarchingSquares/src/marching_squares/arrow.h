#if !defined(ARROW_H)
#define ARROW_H

#include <stdbool.h> // bool
#include "./internal.h" // cell_t
#include "./vertex.h" // NVERTICES
#include "./edge.h" // NEDGES, edge_t

#define NMAXARROWS 2

// datatype to store arrow in cell
typedef struct {
  bool visited;
  const edge_t * edge_tail;
  const edge_t * edge_head;
  // pointer to the adjacent cells,
  //   which can be NULL if the edge is on the boundary
  cell_t * cell_tail;
  cell_t * cell_head;
} arrow_t;

extern int find_arrows_of_cell (
    const double threshold,
    const double vertex_values[NVERTICES],
    edge_t * const edges[NEDGES],
    cell_t * const cells[NEDGES],
    arrow_t * arrows[NMAXARROWS]
);

#endif // ARROW_H
