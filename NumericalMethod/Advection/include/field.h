#if !defined(FIELD_H)
#define FIELD_H

#include "config.h"

extern double * allocate_field (
    const config_t * const config
);

extern void deallocate_field (
    double * const field
);

extern int exchange_halo (
    const config_t * const config,
    double * const field
);

extern int init_vof (
    const config_t * const config,
    const double * const x,
    double * const vof
);

#endif // FIELD_H
