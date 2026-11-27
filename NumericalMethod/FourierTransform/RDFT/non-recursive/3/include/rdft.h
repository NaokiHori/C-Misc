#if !defined(RDFT_H)
#define RDFT_H

#include <stddef.h> // size_t

typedef struct rdft_plan_t rdft_plan_t;

extern int rdft_init_plan(
    const size_t nitems,
    rdft_plan_t ** const plan
);

extern int rdft_destroy_plan(
    rdft_plan_t ** const plan
);

extern int rdft_execute_inplace_forward_transform(
    const rdft_plan_t * const plan,
    double * const values
);

extern int rdft_execute_inplace_backward_transform(
    const rdft_plan_t * const plan,
    double * const values
);

extern int rdft_execute_outofplace_forward_transform(
    const rdft_plan_t * const plan,
    const double * const restrict inputs,
    double * const restrict outputs
);

extern int rdft_execute_outofplace_backward_transform(
    const rdft_plan_t * const plan,
    const double * const restrict inputs,
    double * const restrict outputs
);

#endif // RDFT_H
