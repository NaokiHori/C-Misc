#if !defined(EDGE_H)
#define EDGE_H

#include <stddef.h> // size_t

// simply saying that one cell (square) has four edges
#define NEDGES 4

typedef enum {
  EDGE_Y_NEG = 0,
  EDGE_X_POS = 1,
  EDGE_Y_POS = 2,
  EDGE_X_NEG = 3,
} edge_index_t;

// datatype to store an edge: intersecting point
typedef struct {
  double x;
  double y;
} edge_t;

extern int init_x_edges (
    const double threshold,
    const size_t nx,
    const size_t ny,
    double * const * const grids,
    const double * const field,
    edge_t ** const edges
);

extern int init_y_edges (
    const double threshold,
    const size_t nx,
    const size_t ny,
    double * const * const grids,
    const double * const field,
    edge_t ** const edges
);

#endif // EDGE_H
