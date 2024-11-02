#if !defined(DST_H)
#define DST_H

#include <stddef.h> // size_t

// planner
typedef struct dst_plan_t dst_plan_t;

// create a plan
extern int dst_init_plan (
    const size_t nitems,
    dst_plan_t ** const plan
);

// clean-up a plan
extern int dst_destroy_plan (
    dst_plan_t ** const plan
);

// perform forward transform (DST type 2)
extern int dst_exec_f (
    dst_plan_t * const plan,
    double * const xs
);

// perform backward transform (DST type 3)
extern int dst_exec_b (
    dst_plan_t * const plan,
    double * const xs
);

#endif // DST_H
