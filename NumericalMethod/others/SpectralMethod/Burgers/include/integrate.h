#if !defined(INTEGRATE_H)
#define INTEGRATE_H

#include <stddef.h>
#include <fftw3.h>
#include "my_ffts.h"

#define N_ADVECTIVE_SCHEMES 3

extern int integrate (
    const int nitems,
    const double dt,
    const double nu,
    my_ffts_t * const my_ffts,
    const size_t advective_scheme_index,
    fftw_complex * const gs
);

#endif // INTEGRATE_H
