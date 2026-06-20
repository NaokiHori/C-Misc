#if !defined(SOLVE_LINEAR_SYSTEM_H)
#define SOLVE_LINEAR_SYSTEM_H

#include <stddef.h> // size_t

extern int solve_linear_system(
    const size_t nitems,
    double * const a,
    double * const x
);

#endif // SOLVE_LINEAR_SYSTEM_H
