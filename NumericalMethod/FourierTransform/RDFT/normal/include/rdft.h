#if !defined(RDFT_H)
#define RDFT_H

#include <complex.h> // double complex

// planner
typedef struct rdft_plan_t rdft_plan_t;

// create a plan
extern int rdft_init_plan (
    const size_t nitems,
    rdft_plan_t ** const plan
);

// clean-up a plan
extern int rdft_destroy_plan (
    rdft_plan_t ** const plan
);

// perform forward transform
extern int rdft_exec_f (
    rdft_plan_t * const plan,
    const double * const xs,
    double complex * const ys
);

// perform backward transform
extern int rdft_exec_b (
    rdft_plan_t * const plan,
    const double complex * const xs,
    double * const ys
);

#endif // RDFT_H
