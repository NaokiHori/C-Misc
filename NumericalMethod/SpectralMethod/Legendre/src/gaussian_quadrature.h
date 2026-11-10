#if !defined(GAUSSIAN_QUADRATURE_H)
#define GAUSSIAN_QUADRATURE_H

#include <stddef.h>

extern int find_gaussian_quadrature(
    const size_t nitems,
    double * const roots,
    double * const weights
);

#endif // GAUSSIAN_QUADRATURE_H
