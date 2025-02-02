#include <stddef.h> // size_t
#include <stdbool.h> // bool
#include "./memory.h"
#include "./edge.h"
#include "./square.h"

static double find_intersection(
    const double yt,
    const double * const xs,
    const double * const ys
) {
  // consider a linear function and find x = xt where y = yt:
  //   yt = ys[0] + (ys[1] - ys[0]) / (xs[1] - xs[0]) * (xt - xs[0])
  return 1. / (ys[1] - ys[0]) * (
      + xs[0] * (ys[1] - yt)
      - xs[1] * (ys[0] - yt)
  );
}

static bool is_between(
    const double p,
    const double qs[2]
) {
  if ((qs[0] - p) * (qs[1] - p) <= 0.) {
    return true;
  } else {
    return false;
  }
}

int init_square_x_edges(
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
      const double x = grids[0][i];
      const double ylim[] = {
        grids[1][j    ],
        grids[1][j + 1],
      };
      const double vertex_values[] = {
        field[(j    ) * (nx + 1) + (i    )],
        field[(j + 1) * (nx + 1) + (i    )],
      };
      const double y = find_intersection(threshold, ylim, vertex_values);
      if (is_between(y, ylim)) {
        *edge = memory_alloc(1 * sizeof(edge_t));
        (*edge)->is_diagonal = false;
        (*edge)->x = x;
        (*edge)->y = y;
      } else {
        *edge = NULL;
      }
    }
  }
  return 0;
}

int init_square_y_edges(
    const double threshold,
    const size_t nx,
    const size_t ny,
    double * const * const grids,
    const double * const field,
    edge_t ** const edges
) {
  for (size_t j = 0; j < ny + 1; j++) {
    const double y = grids[1][j];
    for (size_t i = 0; i < nx; i++) {
      edge_t ** const edge = edges + j * (nx + 0) + i;
      const double xlim[] = {
        grids[0][i    ],
        grids[0][i + 1],
      };
      const double vertex_values[] = {
        field[(j    ) * (nx + 1) + (i    )],
        field[(j    ) * (nx + 1) + (i + 1)],
      };
      const double x = find_intersection(threshold, xlim, vertex_values);
      if (is_between(x, xlim)) {
        *edge = memory_alloc(1 * sizeof(edge_t));
        (*edge)->is_diagonal = false;
        (*edge)->x = x;
        (*edge)->y = y;
      } else {
        *edge = NULL;
      }
    }
  }
  return 0;
}

int init_diagonal_edges(
    const double threshold,
    const size_t nx,
    const size_t ny,
    double * const * const grids,
    const double * const field,
    edge_t ** const edges
) {
  for (size_t j = 0; j < ny; j++) {
    for (size_t i = 0; i < nx; i++) {
      const double center_position[] = {
        + 0.5 * grids[0][i    ] + 0.5 * grids[0][i + 1],
        + 0.5 * grids[1][j    ] + 0.5 * grids[1][j + 1],
      };
      const double center_value = get_centered_value((double [NUM_SQUARE_VERTICES]){
          field[(j    ) * (nx + 1) + (i    )],
          field[(j    ) * (nx + 1) + (i + 1)],
          field[(j + 1) * (nx + 1) + (i    )],
          field[(j + 1) * (nx + 1) + (i + 1)],
      });
      for (size_t k = 0; k < NUM_SQUARE_VERTICES; k++) {
        edge_t ** const edge = edges + (j * nx + i) * NUM_SQUARE_VERTICES + k;
        const size_t ii = SQUARE_VERTEX_LEFT_BOTTOM == k || SQUARE_VERTEX_LEFT_TOP     == k ? i : i + 1;
        const size_t jj = SQUARE_VERTEX_LEFT_BOTTOM == k || SQUARE_VERTEX_RIGHT_BOTTOM == k ? j : j + 1;
        const double xlim[] = {
          grids[0][ii],
          center_position[0],
        };
        const double ylim[] = {
          grids[1][jj],
          center_position[1],
        };
        const double vertex_values[] = {
          field[jj * (nx + 1) + ii],
          center_value,
        };
        const double x = find_intersection(threshold, xlim, vertex_values);
        const double y = find_intersection(threshold, ylim, vertex_values);
        if (is_between(x, xlim) && is_between(y, ylim)) {
          *edge = memory_alloc(1 * sizeof(edge_t));
          (*edge)->is_diagonal = true;
          (*edge)->x = x;
          (*edge)->y = y;
        } else {
          *edge = NULL;
        }
      }
    }
  }
  return 0;
}

