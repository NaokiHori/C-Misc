#if !defined(MYDCT_H)
#define MYDCT_H

#include <stddef.h>  // size_t
#include <stdbool.h> // bool

// planner
typedef struct mydct_plan_t mydct_plan_t;

// create a plan
extern int mydct_init_plan (
    const size_t nitems,
    const bool reorder,
    mydct_plan_t ** plan
);

// clean-up a plan
extern int mydct_destroy_plan (
    mydct_plan_t ** plan
);

// perform forward transform (DCT type 2)
extern int mydct_exec_f (
    mydct_plan_t * plan,
    const double * restrict xs,
    double * restrict ys
);

// perform backward transform (DCT type 3)
extern int mydct_exec_b (
    mydct_plan_t * plan,
    const double * restrict xs,
    double * restrict ys
);

#endif // MYDCT_H
