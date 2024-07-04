#if !defined(CELL_H)
#define CELL_H

#include <stddef.h> // size_t
#include "./internal.h" // cell_t
#include "./arrow.h" // arrow_t, NMAXARROWS
#include "./edge.h" // edge_t

struct cell_t {
  arrow_t * arrows[NMAXARROWS];
};

extern int init_cells (
    const double threshold,
    const size_t nx,
    const size_t ny,
    edge_t ** const x_edges,
    edge_t ** const y_edges,
    const double * const field,
    cell_t * const cells
);

#endif // CELL_H
