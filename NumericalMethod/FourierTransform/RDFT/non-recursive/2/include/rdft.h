#if !defined(RDFT_H)
#define RDFT_H

#include <stddef.h>

typedef struct rdft_plan_t rdft_plan_t;

extern int rdft_init_plan(
    const size_t nitems,
    rdft_plan_t ** const plan
);

extern int rdft_destroy_plan(
    rdft_plan_t ** const plan
);

extern int rdft_exec_f(
    rdft_plan_t * const plan,
    double * const xs
);

extern int rdft_exec_b(
    rdft_plan_t * const plan,
    double * const xs
);

#endif // RDFT_H
