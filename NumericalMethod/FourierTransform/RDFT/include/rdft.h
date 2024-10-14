#if !defined(RDFT_H)
#define RDFT_H

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
// xs: x[0], x[1], ..., x[nitems - 1]
extern int rdft_exec_f (
    rdft_plan_t * const plan,
    double * const xs
);

// perform backward transform
// xs: Re(x[0]), Re(x[1]), ..., Re(x[nitems - 1]),
//     Im(x[nitems - 2]), Im(x[nitems - 3]), ..., Im(x[1])
extern int rdft_exec_b (
    rdft_plan_t * const plan,
    double * const xs
);

#endif // RDFT_H
