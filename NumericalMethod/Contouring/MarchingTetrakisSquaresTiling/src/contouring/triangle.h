#if !defined(TRIANGLE_H)
#define TRIANGLE_H

#include "./internal.h" // arrow_t

#define NUM_TRIANGLE_VERTICES 3
#define NUM_TRIANGLE_EDGES 3

// anti-clockwise
typedef enum {
  TRIANGLE_VERTEX_90       = 0,
  TRIANGLE_VERTEX_45_LEFT  = 1,
  TRIANGLE_VERTEX_45_RIGHT = 2,
} triangle_vertex_index_t;

// diagonally-opposite to each vertex defined above
typedef enum {
  TRIANGLE_EDGE_90       = 0,
  TRIANGLE_EDGE_45_LEFT  = 1,
  TRIANGLE_EDGE_45_RIGHT = 2,
} triangle_edge_index_t;

// a triangular element can contain
//   no triangles: NULL
//   one triangle: a pointer to it
typedef struct {
  arrow_t * arrow;
} triangle_t;

extern int init_triangles (
    const double threshold,
    const size_t nx,
    const size_t ny,
    edge_t ** const square_x_edges,
    edge_t ** const square_y_edges,
    edge_t ** const diagonal_edges,
    const double * const field,
    triangle_t * const triangles
);

#endif // TRIANGLE_H
