#if !defined(SDFT_H)
#define SDFT_H

#include <complex.h> // double complex

// planner
typedef struct sdft_plan_t sdft_plan_t;

// create a plan
extern int sdft_init_plan (
    const size_t nitems,
    sdft_plan_t ** const plan
);

// clean-up a plan
extern int sdft_destroy_plan (
    sdft_plan_t ** const plan
);

// perform forward transform
extern int sdft_exec_f (
    sdft_plan_t * const plan,
    double complex * const xs
);

// perform backward transform
extern int sdft_exec_b (
    sdft_plan_t * const plan,
    double complex * const xs
);

#endif // SDFT_H
