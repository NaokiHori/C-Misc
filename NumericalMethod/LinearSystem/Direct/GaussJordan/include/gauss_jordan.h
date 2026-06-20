#if !defined(GAUSS_JORDAN_H)
#define GAUSS_JORDAN_H

#include <stddef.h> // size_t

extern int gauss_jordan (
    const size_t nitems,
    double * const arr,
    double * const inv
);

#endif // GAUSS_JORDAN_H
