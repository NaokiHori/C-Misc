#include <stdio.h> // FILE, fprintf, fclose
#include <stdbool.h> // bool
#include "./internal.h"
#include "./cell.h"
#include "./arrow.h" // arrow_t, NMAXARROWS

int init_cells (
    const double threshold,
    const size_t nx,
    const size_t ny,
    edge_t ** const x_edges,
    edge_t ** const y_edges,
    const double * const field,
    cell_t * const cells
) {
  const bool debug = false;
  for (size_t n = 0; n < nx * ny; n++) {
    cell_t * const cell = cells + n;
    const size_t i = n % nx;
    const size_t j = n / nx;
    // scalar value on the vertices
    double vertex_values[NVERTICES] = {0., 0., 0., 0.};
    vertex_values[VERTEX_LEFT_BOTTOM ] = field[(j    ) * (nx + 1) + (i    )];
    vertex_values[VERTEX_RIGHT_BOTTOM] = field[(j    ) * (nx + 1) + (i + 1)];
    vertex_values[VERTEX_RIGHT_TOP   ] = field[(j + 1) * (nx + 1) + (i + 1)];
    vertex_values[VERTEX_LEFT_TOP    ] = field[(j + 1) * (nx + 1) + (i    )];
    // surrounding edges
    edge_t * surrounding_edges[NEDGES] = {NULL, NULL, NULL, NULL};
    surrounding_edges[EDGE_Y_NEG] = y_edges[(j    ) * (nx + 0) + (i    )];
    surrounding_edges[EDGE_X_POS] = x_edges[(j    ) * (nx + 1) + (i + 1)];
    surrounding_edges[EDGE_Y_POS] = y_edges[(j + 1) * (nx + 0) + (i    )];
    surrounding_edges[EDGE_X_NEG] = x_edges[(j    ) * (nx + 1) + (i    )];
    // neighboring cells
    cell_t * neighboring_cells[NEDGES] = {NULL, NULL, NULL, NULL};
    neighboring_cells[EDGE_Y_NEG] =      0 == j ? NULL : cells + (j - 1) * nx + (i    );
    neighboring_cells[EDGE_X_POS] = nx - 1 == i ? NULL : cells + (j    ) * nx + (i + 1);
    neighboring_cells[EDGE_Y_POS] = ny - 1 == j ? NULL : cells + (j + 1) * nx + (i    );
    neighboring_cells[EDGE_X_NEG] =      0 == i ? NULL : cells + (j    ) * nx + (i - 1);
    find_arrows_of_cell(
        threshold,
        vertex_values,
        surrounding_edges,
        neighboring_cells,
        cell->arrows
    );
  }
  if (debug) {
    FILE * const fp = fopen("arrows.dat", "w");
    for (size_t n = 0; n < nx * ny; n++) {
      const cell_t * const cell = cells + n;
      const arrow_t * const arrows[NMAXARROWS] = {
        cell->arrows[0],
        cell->arrows[1],
      };
      for (size_t m = 0; m < sizeof(arrows) / sizeof(arrows[0]); m++) {
        const arrow_t * const arrow = arrows[m];
        if (NULL == arrow) {
          continue;
        }
        fprintf(fp, "% .7e % .7e\n", arrow->edge_tail->x, arrow->edge_tail->y);
        fprintf(fp, "% .7e % .7e\n", arrow->edge_head->x, arrow->edge_head->y);
        fprintf(fp, "\n");
      }
    }
    fclose(fp);
  }
  return 0;
}

