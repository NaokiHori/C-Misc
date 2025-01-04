#if !defined(DFT_H)
#define DFT_H

#include <complex.h> // double complex

// planner
typedef struct dft_plan_t dft_plan_t;

// create a plan
extern int dft_init_plan (
    const size_t nitems,
    dft_plan_t ** const plan
);

// clean-up a plan
extern int dft_destroy_plan (
    dft_plan_t ** const plan
);

// perform forward transform
extern int dft_exec_f (
    dft_plan_t * const plan,
    double complex * const xs
);

// perform backward transform
extern int dft_exec_b (
    dft_plan_t * const plan,
    double complex * const xs
);

#endif // DFT_H
