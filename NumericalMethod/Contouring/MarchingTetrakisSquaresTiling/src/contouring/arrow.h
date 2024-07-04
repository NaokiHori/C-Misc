#if !defined(ARROW_H)
#define ARROW_H

#include <stdbool.h> // bool
#include "./edge.h" // edge_t
#include "./triangle.h" // triangle_t, NUM_TRIANGLE_VERTICES, NUM_TRIANGLE_EDGES

// datatype to store arrow in triangle
struct arrow_t {
  bool visited;
  const edge_t * edge_tail;
  const edge_t * edge_head;
  // pointer to the adjacent triangles,
  //   which can be NULL if the edge is on the boundary
  triangle_t * triangle_tail;
  triangle_t * triangle_head;
};

extern int find_arrow_in_triangle (
    const double threshold,
    const double vertex_values[NUM_TRIANGLE_VERTICES],
    const edge_t * const edges[NUM_TRIANGLE_EDGES],
    triangle_t * const triangles[NUM_TRIANGLE_EDGES],
    arrow_t ** const arrow
);

#endif // ARROW_H
