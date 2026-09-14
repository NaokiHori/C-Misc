#if !defined(MODIFIED_CHOLESKY_H)
#define MODIFIED_CHOLESKY_H

#include <stddef.h>

extern int modified_cholesky (
    const size_t nitems,
    double * const a,
    double * const x
);

#endif // MODIFIED_CHOLESKY_H
