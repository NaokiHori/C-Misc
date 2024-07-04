#if !defined(INTERP_CUBIC_H)
#define INTERP_CUBIC_H

#include <stddef.h> // size_t

typedef struct interp_spectral_plan_t interp_spectral_plan_t;

extern int interp_spectral_initialise(
    const size_t n1,
    const size_t n2,
    interp_spectral_plan_t ** plan
);

extern int interp_spectral_execute(
    interp_spectral_plan_t * plan,
    const double * y1s,
    double * y2s
);

extern int interp_spectral_finalise(
    interp_spectral_plan_t ** plan
);

#endif // INTERP_CUBIC_H
