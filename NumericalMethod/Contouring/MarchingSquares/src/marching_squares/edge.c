#include <stdio.h> // FILE etc.
#include <stddef.h> // size_t
#include <stdbool.h> // bool
#include "./memory.h"
#include "./edge.h"

static const bool debug = false;

static double find_intersection (
    const double yt,
    const double * xs,
    const double * ys
) {
  // consider a linear function and find x where y = yt
  return 1. / (ys[1] - ys[0]) * (
      + xs[0] * (- yt + ys[1])
      + xs[1] * (+ yt - ys[0])
  );
}

int init_x_edges (
    const double threshold,
    const size_t nx,
    const size_t ny,
    double * const * const grids,
    const double * const field,
    edge_t ** const edges
) {
  for (size_t j = 0; j < ny; j++) {
    for (size_t i = 0; i < nx + 1; i++) {
      edge_t ** const edge = edges + j * (nx + 1) + i;
      const double positions[] = {
        grids[1][j    ],
        grids[1][j + 1],
      };
      const double vertex_values[] = {
        field[(j    ) * (nx + 1) + (i    )],
        field[(j + 1) * (nx + 1) + (i    )],
      };
      const double intersection = find_intersection(threshold, positions, vertex_values);
      if (positions[0] <= intersection && intersection <= positions[1]) {
        *edge = memory_alloc(1 * sizeof(edge_t));
        double * const x = &(*edge)->x;
        double * const y = &(*edge)->y;
        *x = grids[0][i];
        *y = intersection;
      } else {
        *edge = NULL;
      }
    }
  }
  if (debug) {
    FILE * const fp = fopen("x_intersections.dat", "w");
    for (size_t j = 0; j < ny; j++) {
      for (size_t i = 0; i < nx + 1; i++) {
        const edge_t * const edge = edges[j * (nx + 1) + i];
        if (NULL != edge) {
          const double x = edge->x;
          const double y = edge->y;
          fprintf(fp, "% .15e % .15e\n", x, y);
        }
      }
    }
    fclose(fp);
  }
  return 0;
}

int init_y_edges (
    const double threshold,
    const size_t nx,
    const size_t ny,
    double * const * const grids,
    const double * const field,
    edge_t ** const edges
) {
  for (size_t j = 0; j < ny + 1; j++) {
    for (size_t i = 0; i < nx; i++) {
      edge_t ** const edge = edges + j * (nx + 0) + i;
      const double positions[] = {
        grids[0][i    ],
        grids[0][i + 1],
      };
      const double vertex_values[] = {
        field[(j    ) * (nx + 1) + (i    )],
        field[(j    ) * (nx + 1) + (i + 1)],
      };
      const double intersection = find_intersection(threshold, positions, vertex_values);
      if (positions[0] <= intersection && intersection <= positions[1]) {
        *edge = memory_alloc(1 * sizeof(edge_t));
        double * const x = &(*edge)->x;
        double * const y = &(*edge)->y;
        *x = intersection;
        *y = grids[1][j];
      } else {
        *edge = NULL;
      }
    }
  }
  if (debug) {
    FILE * const fp = fopen("y_intersections.dat", "w");
    for (size_t j = 0; j < ny + 1; j++) {
      for (size_t i = 0; i < nx; i++) {
        const edge_t * const edge = edges[j * (nx + 0) + i];
        if (NULL != edge) {
          const double x = edge->x;
          const double y = edge->y;
          fprintf(fp, "% .15e % .15e\n", x, y);
        }
      }
    }
    fclose(fp);
  }
  return 0;
}

