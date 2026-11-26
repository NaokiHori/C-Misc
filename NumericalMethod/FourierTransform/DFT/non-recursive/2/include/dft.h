#if !defined(DFT_H)
#define DFT_H

#include <stddef.h> // size_t

typedef struct dft_plan_t dft_plan_t;

extern int dft_init_plan(
    const size_t nitems,
    dft_plan_t ** const plan
);

extern int dft_destroy_plan(
    dft_plan_t ** const plan
);

extern int dft_execute_inplace_forward_transform(
    const dft_plan_t * const plan,
    double * const restrict reals,
    double * const restrict imags
);

extern int dft_execute_inplace_backward_transform(
    const dft_plan_t * const plan,
    double * const restrict reals,
    double * const restrict imags
);

extern int dft_execute_outofplace_forward_transform(
    const dft_plan_t * const plan,
    const double * const restrict input_reals,
    const double * const restrict input_imags,
    double * const restrict output_reals,
    double * const restrict output_imags
);

extern int dft_execute_outofplace_backward_transform(
    const dft_plan_t * const plan,
    const double * const restrict input_reals,
    const double * const restrict input_imags,
    double * const restrict output_reals,
    double * const restrict output_imags
);

#endif // DFT_H
