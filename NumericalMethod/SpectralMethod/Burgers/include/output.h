#if !defined(OUTPUT_H)
#define OUTPUT_H

#include <stddef.h>
#include <fftw3.h>

extern int output (
    const size_t counter,
    const int nitems,
    const size_t advective_scheme_index,
    const double * const xs,
    const double * const fs
);

#endif // OUTPUT_H
