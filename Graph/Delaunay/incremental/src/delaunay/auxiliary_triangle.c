#include <float.h>
#include <math.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include "./auxiliary_triangle.h"

int get_auxiliary_triangle(
    const size_t num_points,
    const point_t * const points,
    point_t * const vertices,
    triangle_node_t ** const triangle_list_head
) {
  point_t min = {
    .x = + DBL_MAX,
    .y = + DBL_MAX,
  };
  point_t max = {
    .x = - DBL_MAX,
    .y = - DBL_MAX,
  };
  for (size_t i = 0; i < num_points; i++) {
    const point_t * const point = points + i;
    min.x = fmin(min.x, point->x);
    max.x = fmax(max.x, point->x);
    min.y = fmin(min.y, point->y);
    max.y = fmax(max.y, point->y);
  }
  const point_t middle = {
    .x = 0.5 * min.x + 0.5 * max.x,
    .y = 0.5 * min.y + 0.5 * max.y,
  };
  const double delta = fmax(
      max.x - min.x,
      max.y - min.y
  );
  vertices[0].x = middle.x - 20. * delta;
  vertices[0].y = middle.y - delta;
  vertices[1].x = middle.x;
  vertices[1].y = middle.y + 20. * delta;
  vertices[2].x = middle.x + 20. * delta;
  vertices[2].y = middle.y - delta;
  // create a new triangle node and update the root of the edge list
  triangle_node_t * const new_triangle_node = malloc(sizeof(triangle_node_t));
  memcpy(
      new_triangle_node,
      &(triangle_node_t){
        .triangle = {
          vertices + 0,
          vertices + 1,
          vertices + 2,
        },
        .next = NULL,
      },
      sizeof(triangle_node_t)
  );
  *triangle_list_head = new_triangle_node;
  return 0;
}

/**
 * Remove triangles involving auxiliary triangle vertices
 */
int remove_auxiliary_triangles(
    const point_t * const vertices,
    triangle_node_t ** triangle_node
) {
  while (*triangle_node) {
    const triangle_t * const triangle = &(*triangle_node)->triangle;
    bool to_be_removed = false;
    for (size_t i = 0; i < TRIANGLE; i++) {
      const point_t * const point = (*triangle)[i];
      if (
          point == vertices + 0
          ||
          point == vertices + 1
          ||
          point == vertices + 2
      ) {
        to_be_removed = true;
        break;
      }
    }
    if (to_be_removed) {
      triangle_node_t * const next = (*triangle_node)->next;
      free(*triangle_node);
      *triangle_node = next;
    } else {
      triangle_node = &(*triangle_node)->next;
    }
  }
  return 0;
}

