#if !defined(TEST_H)
#define TEST_H

#include <stddef.h>

extern int rdft_naive_exec_f(
    const size_t nitems,
    double * const xs
);

extern int rdft_naive_exec_b(
    const size_t nitems,
    double * const xs
);

#endif // TEST_H
