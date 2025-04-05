#if !defined(DELAUNAY_H)
#define DELAUNAY_H

#include <stddef.h>

#define TRIANGLE 3

typedef struct {
  double x;
  double y;
} point_t;

typedef const point_t * triangle_t[TRIANGLE];

typedef struct triangle_node_t {
  const triangle_t triangle;
  struct triangle_node_t * next;
} triangle_node_t;

extern int delaunay_triangulate(
    const size_t num_points,
    const point_t * const points,
    triangle_node_t ** triangle_node
);

extern int delaunay_check(
    const size_t num_points,
    const point_t * const points,
    const triangle_node_t * triangle_node
);

#endif // DELAUNAY_H
