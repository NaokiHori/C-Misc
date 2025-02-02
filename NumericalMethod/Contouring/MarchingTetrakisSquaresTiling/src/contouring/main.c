#include <stdio.h> // puts
#include <stddef.h> // size_t
#include <stdbool.h> // bool
#include "contouring.h"
#include "./types.h"
#include "./memory.h"
#include "./edge.h"
#include "./arrow.h"
#include "./square.h"

static int add_edge_to_contour(
    const bool to_tail,
    contour_t * const contour,
    const edge_t * const edge
) {
  if (edge->is_diagonal) {
    // intersections with diagonal edge are for implementation ease
    //   and are not included in the resulting series of points
    return 0;
  }
  point_t * const new_point = memory_alloc(1 * sizeof(point_t));
  new_point->x = edge->x;
  new_point->y = edge->y;
  if (to_tail) {
    new_point->next = NULL;
    point_t ** const last_point = &contour->internal->last_point;
    // update last point
    (*last_point)->next = new_point;
    *last_point = new_point;
  } else {
    point_t ** const first_point = &contour->point;
    if (NULL == *first_point) {
      contour->internal->last_point = new_point;
    }
    // update first point
    new_point->next = *first_point;
    *first_point = new_point;
  }
  contour->npoints += 1;
  return 0;
}

static int march(
    triangle_t * const triangle_start,
    arrow_t * const arrow_start,
    contour_t * const contour
) {
  // marching to the direction of head, until
  //   1. we reach one of the boundaries
  //      -> this is a non-closed contour
  //         go back to the start and keep marching to the direction of tail
  //         until we hit another wall
  //   2. we come back to this triangle
  //      -> this is a closed contour
  //         it's done, and thus we terminate
  // for case 1, we keep the starting triangle
  bool reverse = false;
  const edge_t * edge = arrow_start->edge_tail;
  triangle_t * triangle = triangle_start;
  while (1) {
    // from the arrows in the triangle,
    //   pick-up one which intersects with the given edge
    arrow_t * const arrow = triangle->arrow;
    if (NULL == arrow) {
      puts("failed to find an arrow");
      return 1;
    }
    if (edge != (reverse ? arrow->edge_head : arrow->edge_tail)) {
      puts("the arrow is not connected");
      return 1;
    }
    arrow->visited = true;
    // add a new point to the contour
    edge = reverse ? arrow->edge_tail : arrow->edge_head;
    add_edge_to_contour(reverse, contour, edge);
    // move to the triangle which is sitting in front
    triangle = reverse ? arrow->triangle_tail : arrow->triangle_head;
    // check termination
    if (triangle_start == triangle) {
      // now we are at the original triangle: a closed loop is found
      contour->is_closed = true;
      break;
    }
    if (NULL == triangle) {
      // we hit one of the walls
      if (reverse) {
        // second hit, indicating that
        //   a contour which connects two boundaries is found
        contour->is_closed = false;
        break;
      } else {
        // first hit, indicating that this contour is not closed
        // go back to the triangle we start this marching
        //   and move in the opposite direction
        //   to find the other side of contour
        reverse = true;
        edge = arrow_start->edge_head;
        triangle = triangle_start;
      }
    }
  }
  return 0;
}

int contouring_exec(
    const double threshold,
    const size_t * sizes,
    double * const * const grids,
    const double * const field,
    contouring_contour_t ** const contour
) {
  // number of squares for each direction: (nx, ny)
  //   is smaller by 1 than the number of grids: (sizes[0], sizes[1])
  const size_t nx = sizes[0] - 1;
  const size_t ny = sizes[1] - 1;
  const size_t n_squares = nx * ny;
  const size_t n_triangles = NUM_SQUARE_EDGES * n_squares;
  // find all intersections on square / triangle edges a priori
  edge_t ** const square_x_edges = memory_alloc((nx + 1) * (ny + 0) * sizeof(edge_t *));
  edge_t ** const square_y_edges = memory_alloc((nx + 0) * (ny + 1) * sizeof(edge_t *));
  edge_t ** const diagonal_edges = memory_alloc(NUM_SQUARE_VERTICES * n_squares * sizeof(edge_t *));
  init_square_x_edges(threshold, nx, ny, grids, field, square_x_edges);
  init_square_y_edges(threshold, nx, ny, grids, field, square_y_edges);
  init_diagonal_edges(threshold, nx, ny, grids, field, diagonal_edges);
  // initialize triangles
  triangle_t * const triangles = memory_alloc(n_triangles * sizeof(triangle_t));
  if (0 != init_triangles(threshold, nx, ny, square_x_edges, square_y_edges, diagonal_edges, field, triangles)) {
    puts("failed to initialize triangles");
    goto abort;
  }
  // find a triangle containing a contour,
  //   which becomes a starting point to march
  for (size_t n = 0; n < n_triangles; n++) {
    triangle_t * const triangle = triangles + n;
    arrow_t * const arrow = triangle->arrow;
    if (NULL == arrow) {
      continue;
    }
    if (arrow->visited) {
      continue;
    }
    // now it turned out that this triangle contains an unvisited arrow,
    //   indicating a new contour is found
    contour_t * const new_contour = memory_alloc(1 * sizeof(contour_t));
    new_contour->is_closed = false;
    new_contour->npoints = 0;
    new_contour->point = NULL;
    new_contour->internal = memory_alloc(1 * sizeof(contour_internal_t));
    new_contour->internal->last_point = NULL;
    // update root
    new_contour->next = *contour;
    *contour = new_contour;
    if (0 != march(triangle, arrow, new_contour)) {
      puts("failed to march");
      goto abort;
    }
  }
abort:
  for (size_t n = 0; n < n_triangles; n++) {
    triangle_t * const triangle = triangles + n;
    memory_free(triangle->arrow);
  }
  memory_free(triangles);
  for (size_t n = 0; n < (nx + 1) * (ny + 0); n++) {
    edge_t * const edge = square_x_edges[n];
    memory_free(edge);
  }
  for (size_t n = 0; n < (nx + 0) * (ny + 1); n++) {
    edge_t * const edge = square_y_edges[n];
    memory_free(edge);
  }
  for (size_t n = 0; n < NUM_SQUARE_VERTICES * n_squares; n++) {
    edge_t * const edge = diagonal_edges[n];
    memory_free(edge);
  }
  memory_free(square_x_edges);
  memory_free(square_y_edges);
  memory_free(diagonal_edges);
  return 0;
}

int contouring_cleanup(
    contouring_contour_t ** const contour
) {
  while (*contour) {
    point_t * point = (*contour)->point;
    while (point) {
      point_t * const next = point->next;
      memory_free(point);
      point = next;
    }
    memory_free((*contour)->internal);
    contour_t * const next = (*contour)->next;
    memory_free(*contour);
    *contour = next;
  }
  return 0;
}

