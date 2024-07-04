#include <stdio.h> // puts
#include <stddef.h> // size_t
#include <stdbool.h> // bool
#include "marching_squares.h"
#include "./internal.h"
#include "./memory.h"
#include "./edge.h"
#include "./arrow.h"
#include "./cell.h"

static int add_point_to_contour (
    const bool to_tail,
    contour_t * const contour,
    const edge_t * const edge
) {
  point_t * const new_point = memory_alloc(1 * sizeof(point_t));
  new_point->x = edge->x;
  new_point->y = edge->y;
  if (to_tail) {
    new_point->next = NULL;
    point_t * last_point = contour->point;
    while (last_point->next) {
      last_point = last_point->next;
    }
    last_point->next = new_point;
  } else {
    point_t ** const first_point = &contour->point;
    // update root
    new_point->next = *first_point;
    *first_point = new_point;
  }
  return 0;
}

static int march (
    cell_t * const cell_start,
    arrow_t * const arrow_start,
    contour_t * const contour
) {
  // marching to the direction of head, until
  //   1. we reach one of the boundaries
  //      -> this is a non-closed contour
  //         go back to the start and keep marching to the direction of tail
  //         until we hit another wall
  //   2. we come back to this cell
  //      -> this is a closed contour
  //         it's done, and thus we terminate
  // for case 1, we keep the starting cell
  bool reverse = false;
  cell_t * cell = cell_start;
  const edge_t * edge = arrow_start->edge_tail;
  while (1) {
    // from the arrows in the cell,
    //   pick-up one which intersects with the given edge
    arrow_t * arrow = NULL;
    arrow = cell->arrows[0];
    if (NULL == arrow) {
      puts("failed to find an arrow");
      return 1;
    }
    if (edge != (reverse ? arrow->edge_head : arrow->edge_tail)) {
      arrow = cell->arrows[1];
      if (NULL == arrow) {
        puts("only one arrow exists, which are not connected");
        return 1;
      }
      if (edge != (reverse ? arrow->edge_head : arrow->edge_tail)) {
        puts("two arrows exist, both of which are not connected");
        return 1;
      }
    }
    arrow->visited = true;
    // add a new point to the contour
    edge = reverse ? arrow->edge_tail : arrow->edge_head;
    add_point_to_contour(reverse, contour, edge);
    // move to the cell which is sitting in front
    cell = reverse ? arrow->cell_tail : arrow->cell_head;
    // check termination
    if (cell_start == cell) {
      // now we are at the original cell: a closed loop is found
      break;
    }
    if (NULL == cell) {
      // we hit one of the walls
      if (reverse) {
        // second hit, indicating that
        //   a contour which connects two boundaries is found
        break;
      } else {
        // first hit, indicating that this contour is not closed
        // go back to the cell we start this marching
        //   and move in the opposite direction
        //   to find the other side of contour
        reverse = true;
        cell = cell_start;
        edge = arrow_start->edge_head;
      }
    }
  }
  return 0;
}

int marching_squares_exec (
    const double threshold,
    const size_t * sizes,
    double * const * const grids,
    const double * const field,
    marching_squares_contour_t ** const contour
) {
  // number of cells for each direction (nx, ny)
  //   is smaller then the number of grids (sizes[0], sizes[1]) by 1
  const size_t nx = sizes[0] - 1;
  const size_t ny = sizes[1] - 1;
  // find all intersections to initialize x_edges and y_edges
  edge_t ** const x_edges = memory_alloc((nx + 1) * (ny + 0) * sizeof(edge_t *));
  edge_t ** const y_edges = memory_alloc((nx + 0) * (ny + 1) * sizeof(edge_t *));
  init_x_edges(threshold, nx, ny, grids, field, x_edges);
  init_y_edges(threshold, nx, ny, grids, field, y_edges);
  // initialize cells
  cell_t * const cells = memory_alloc(nx * ny * sizeof(cell_t));
  init_cells(threshold, nx, ny, x_edges, y_edges, field, cells);
  // find a cell containing a contour,
  //   which becomes a starting point to march
  for (size_t n = 0; n < nx * ny; n++) {
    cell_t * const cell = cells + n;
    for (size_t m = 0; m < NMAXARROWS; m++) {
      arrow_t * const arrow = cell->arrows[m];
      if (NULL == arrow) {
        continue;
      }
      if (arrow->visited) {
        continue;
      }
      // now it turned out that this cell contains an unvisited arrow,
      //   indicating a new contour is found
      contour_t * const new_contour = memory_alloc(1 * sizeof(contour_t));
      new_contour->point = NULL;
      new_contour->next = *contour;
      *contour = new_contour;
      if (0 != march(cell, arrow, new_contour)) {
        puts("failed to march");
        goto abort;
      }
    }
  }
abort:
  for (size_t n = 0; n < nx * ny; n++) {
    cell_t * const cell = cells + n;
    for (size_t m = 0; m < NMAXARROWS; m++) {
      memory_free(cell->arrows[m]);
    }
  }
  memory_free(cells);
  for (size_t n = 0; n < (nx + 1) * (ny + 0); n++) {
    edge_t * const edge = x_edges[n];
    memory_free(edge);
  }
  for (size_t n = 0; n < (nx + 0) * (ny + 1); n++) {
    edge_t * const edge = y_edges[n];
    memory_free(edge);
  }
  memory_free(x_edges);
  memory_free(y_edges);
  return 0;
}

int marching_squares_cleanup (
    marching_squares_contour_t ** const contour
) {
  while (*contour) {
    point_t * point = (*contour)->point;
    while (point) {
      point_t * next = point->next;
      memory_free(point);
      point = next;
    }
    contour_t * next = (*contour)->next;
    memory_free(*contour);
    *contour = next;
  }
  return 0;
}

