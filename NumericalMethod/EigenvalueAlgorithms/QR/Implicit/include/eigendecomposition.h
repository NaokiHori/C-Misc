#if !defined(EIGENDECOMPOSITION_H)
#define EIGENDECOMPOSITION_H

#include <stddef.h> // size_t

extern int compute_eigendecomposition(
    const size_t nitems,
    double * const main_diagonals,
    double * const sub_diagonals,
    double * const eigenvectors_transposed
);

#endif // EIGENDECOMPOSITION_H
