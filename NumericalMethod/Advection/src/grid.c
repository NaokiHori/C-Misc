#include <assert.h>
#include "grid.h"

int init_grid (
    const config_t * const config,
    double * const x
) {
  // cell-face positions
  const size_t nitems = config->nitems;
  const double dx = compute_dx(config);
  for (size_t i = 1; i <= nitems + 1; i++) {
    x[i] = 0.5 * (2 * i - 2) * dx;
  }
  return 0;
}

int compute_cell_center (
    const config_t * const config,
    const size_t index,
    const double * const x,
    double * const center
) {
  const size_t nitems = config->nitems;
  assert (1 <= index && index <= nitems);
  *center = 0.5 * x[index] + 0.5 * x[index + 1];
  return 0;
}

