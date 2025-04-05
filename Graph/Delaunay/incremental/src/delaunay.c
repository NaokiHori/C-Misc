#include "delaunay.h"
#include "./delaunay/add_point.h"
#include "./delaunay/auxiliary_triangle.h"
#include "./delaunay/circumcircle.h"

int delaunay_triangulate(
    const size_t num_points,
    const point_t * const points,
    triangle_node_t ** triangle_list_head
) {
  // prepare an auxiliary triangle which contains all points
  point_t auxiliary_points[TRIANGLE] = {0};
  if (0 != get_auxiliary_triangle(num_points, points, auxiliary_points, triangle_list_head)) {
    return 1;
  }
  // add each point and perform re-triangulation
  for (size_t i = 0; i < num_points; i++) {
    if (0 != add_point(points + i, triangle_list_head)) {
      return 1;
    }
  }
  // remove all triangles which are composed by the auxiliary points
  if (0 != remove_auxiliary_triangles(auxiliary_points, triangle_list_head)) {
    return 1;
  }
  return 0;
}

/**
 * Check if the given triangles satisfy Delaunay triangulation by definition.
 *
 * Check naively: for each triangle, visit all points to see if all points are outside the triangle
 */
int delaunay_check(
    const size_t num_points,
    const point_t * const points,
    const triangle_node_t * triangle_node
) {
  while (triangle_node) {
    const triangle_t * const triangle = &triangle_node->triangle;
    for (size_t i = 0; i < num_points; i++) {
      const point_t * const point = points + i;
      // skip points which is a member of this triangle
      if (
          point == (*triangle)[0]
          ||
          point == (*triangle)[1]
          ||
          point == (*triangle)[2]
      ) {
        continue;
      }
      if (point_is_in_circumcircle(point, triangle)) {
        return 1;
      }
    }
    triangle_node = triangle_node->next;
  }
  return 0;
}

