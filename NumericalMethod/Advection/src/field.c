#include <stddef.h> // size_t
#include <stdlib.h> // malloc, free
#include <assert.h>
#include "grid.h"
#include "field.h"

double * allocate_field (
    const config_t * const config
) {
  const size_t nitems = config->nitems;
  double * const field = malloc((nitems + 2) * sizeof(double));
  assert(NULL != field);
  return field;
}

void deallocate_field (
    double * const field
) {
  free(field);
}

int exchange_halo (
    const config_t * const config,
    double * const field
) {
  const size_t nitems = config->nitems;
  // periodic
  field[         0] = field[nitems];
  field[nitems + 1] = field[     1];
  return 0;
}

int init_vof (
    const config_t * const config,
    const double * const x,
    double * const vof
) {
  const double center_t0 = 2.;
  const double width = 1.;
  const size_t nitems = config->nitems;
  for (size_t i = 1; i <= nitems; i++) {
    double cell_center = 0.;
    assert(0 == compute_cell_center(config, i, x, &cell_center));
    const double xl = center_t0 - 0.5 * width;
    const double xr = center_t0 + 0.5 * width;
    vof[i] = xl < cell_center && cell_center < xr ? 1.0 : 0.0;
  }
  exchange_halo(config, vof);
  return 0;
}

