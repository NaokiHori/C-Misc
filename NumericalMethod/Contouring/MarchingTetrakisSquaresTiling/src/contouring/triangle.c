#include <stdio.h> // FILE, fprintf, fclose
#include <stdbool.h> // bool
#include "./internal.h"
#include "./arrow.h" // arrow_t, NMAXARROWS
#include "./square.h"
#include "./triangle.h"

int init_triangles (
    const double threshold,
    const size_t nx,
    const size_t ny,
    edge_t ** const square_x_edges,
    edge_t ** const square_y_edges,
    edge_t ** const diagonal_edges,
    const double * const field,
    triangle_t * const triangles
) {
  const bool debug = false;
  for (size_t n = 0; n < nx * ny; n++) {
    const size_t i = n % nx;
    const size_t j = n / nx;
    // pack the following to seek for an arrow in four triangles
    //   1. scalar value on the vertices
    //   2. surrounding edges
    //   3. neighboring triangles
    const double square_vertex_values[NUM_SQUARE_VERTICES] = {
      field[(j    ) * (nx + 1) + (i    )],
      field[(j    ) * (nx + 1) + (i + 1)],
      field[(j + 1) * (nx + 1) + (i + 1)],
      field[(j + 1) * (nx + 1) + (i    )],
    };
    const double center_value = get_centered_value((double [NUM_SQUARE_VERTICES]){
        square_vertex_values[SQUARE_VERTEX_LEFT_BOTTOM ],
        square_vertex_values[SQUARE_VERTEX_RIGHT_BOTTOM],
        square_vertex_values[SQUARE_VERTEX_RIGHT_TOP   ],
        square_vertex_values[SQUARE_VERTEX_LEFT_TOP    ],
    });
    const edge_t * const external_edges[NUM_SQUARE_EDGES] = {
      square_y_edges[(j    ) * (nx + 0) + (i    )],
      square_x_edges[(j    ) * (nx + 1) + (i + 1)],
      square_y_edges[(j + 1) * (nx + 0) + (i    )],
      square_x_edges[(j    ) * (nx + 1) + (i    )],
    };
    const edge_t * const internal_edges[NUM_SQUARE_VERTICES] = {
      diagonal_edges[(j * nx + i) * NUM_SQUARE_VERTICES + SQUARE_VERTEX_LEFT_BOTTOM ],
      diagonal_edges[(j * nx + i) * NUM_SQUARE_VERTICES + SQUARE_VERTEX_RIGHT_BOTTOM],
      diagonal_edges[(j * nx + i) * NUM_SQUARE_VERTICES + SQUARE_VERTEX_RIGHT_TOP   ],
      diagonal_edges[(j * nx + i) * NUM_SQUARE_VERTICES + SQUARE_VERTEX_LEFT_TOP    ],
    };
    triangle_t * const external_triangles[NUM_SQUARE_EDGES] = {
           0 == j ? NULL : triangles + ((j - 1) * nx + (i    )) * NUM_SQUARE_VERTICES + SQUARE_EDGE_YPOS,
      nx - 1 == i ? NULL : triangles + ((j    ) * nx + (i + 1)) * NUM_SQUARE_VERTICES + SQUARE_EDGE_XNEG,
      ny - 1 == j ? NULL : triangles + ((j + 1) * nx + (i    )) * NUM_SQUARE_VERTICES + SQUARE_EDGE_YNEG,
           0 == i ? NULL : triangles + ((j    ) * nx + (i - 1)) * NUM_SQUARE_VERTICES + SQUARE_EDGE_XPOS,
    };
    triangle_t * const internal_triangles[NUM_SQUARE_EDGES] = {
      triangles + (j * nx + i) * NUM_SQUARE_VERTICES + SQUARE_EDGE_YNEG,
      triangles + (j * nx + i) * NUM_SQUARE_VERTICES + SQUARE_EDGE_XPOS,
      triangles + (j * nx + i) * NUM_SQUARE_VERTICES + SQUARE_EDGE_YPOS,
      triangles + (j * nx + i) * NUM_SQUARE_VERTICES + SQUARE_EDGE_XNEG,
    };
    for (size_t k = 0; k < NUM_SQUARE_EDGES; k++) {
      triangle_t * const triangle = triangles + NUM_SQUARE_EDGES * n + k;
      const double vertex_values[NUM_TRIANGLE_VERTICES] = {
        center_value,
        square_vertex_values[(k + 0) % NUM_SQUARE_VERTICES],
        square_vertex_values[(k + 1) % NUM_SQUARE_VERTICES],
      };
      const edge_t * const surrounding_edges[NUM_TRIANGLE_EDGES] = {
        external_edges[k],
        internal_edges[(k + 1) % NUM_SQUARE_VERTICES],
        internal_edges[(k + 0) % NUM_SQUARE_VERTICES],
      };
      triangle_t * const neighboring_triangles[NUM_TRIANGLE_EDGES] = {
        external_triangles[k],
        internal_triangles[(k + 1) % NUM_SQUARE_EDGES],
        internal_triangles[(k + 3) % NUM_SQUARE_EDGES],
      };
      if (0 != find_arrow_in_triangle(
          threshold,
          vertex_values,
          surrounding_edges,
          neighboring_triangles,
          &triangle->arrow
      )) {
        printf("failed to find arrow: %zu, %zu, %zu\n", i, j, k);
        return 1;
      }
    }
  }
  if (debug) {
    FILE * const fp = fopen("arrows.dat", "w");
    for (size_t n = 0; n < NUM_SQUARE_VERTICES * nx * ny; n++) {
      const arrow_t * const arrow = triangles[n].arrow;
      if (NULL == arrow) {
        continue;
      }
      fprintf(fp, "% .7e % .7e\n", arrow->edge_tail->x, arrow->edge_tail->y);
      fprintf(fp, "% .7e % .7e\n", arrow->edge_head->x, arrow->edge_head->y);
      fprintf(fp, "\n");
    }
    fclose(fp);
  }
  return 0;
}

