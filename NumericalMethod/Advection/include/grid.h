#if !defined(GRID_H)
#define GRID_H

#include "config.h"

int init_grid (
    const config_t * const config,
    double * const x
);

int compute_cell_center (
    const config_t * const config,
    const size_t index,
    const double * const x,
    double * const center
);

#endif // GRID_H
