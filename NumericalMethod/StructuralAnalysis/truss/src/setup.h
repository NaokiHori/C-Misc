#if !defined(SETUP_H)
#define SETUP_H

#include <stddef.h>
#include "common.h"

extern int setup(
    size_t * const n_nodes,
    node_t ** const nodes,
    size_t * const n_elements,
    element_t ** const elements,
    double ** const external_forces
);

#endif // SETUP_H
