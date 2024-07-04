#if !defined(INTERP_CUBIC_H)
#define INTERP_CUBIC_H

#include <stddef.h> // size_t

typedef struct interp_cubic_plan_t interp_cubic_plan_t;

extern int interp_cubic_initialise (
    const size_t n1,
    const size_t n2,
    const double * x1s,
    const double * x2s,
    interp_cubic_plan_t ** plan
);

extern int interp_cubic_execute (
    interp_cubic_plan_t * plan,
    const double * y1s,
    double * y2s
);

extern int interp_cubic_finalise (
    interp_cubic_plan_t ** plan
);

#endif // INTERP_CUBIC_H
