#if !defined(GAUSSIAN_ELIMINATION_H)
#define GAUSSIAN_ELIMINATION_H

#include <stddef.h> // size_t

extern int gaussian_elimination (
    const size_t nitems,
    double * const a,
    double * const x
);

#endif // GAUSSIAN_ELIMINATION_H
