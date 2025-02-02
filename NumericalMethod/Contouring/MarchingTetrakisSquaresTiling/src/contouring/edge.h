#if !defined(EDGE_H)
#define EDGE_H

#include <stddef.h> // size_t
#include <stdbool.h> // bool

// datatype to store an edge: intersecting point
typedef struct {
  bool is_diagonal;
  double x;
  double y;
} edge_t;

extern int init_square_x_edges(
    const double threshold,
    const size_t nx,
    const size_t ny,
    double * const * const grids,
    const double * const field,
    edge_t ** const edges
);

extern int init_square_y_edges(
    const double threshold,
    const size_t nx,
    const size_t ny,
    double * const * const grids,
    const double * const field,
    edge_t ** const edges
);

extern int init_diagonal_edges(
    const double threshold,
    const size_t nx,
    const size_t ny,
    double * const * const grids,
    const double * const field,
    edge_t ** const edges
);

#endif // EDGE_H
