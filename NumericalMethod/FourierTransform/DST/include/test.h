#if !defined(TEST_H)
#define TEST_H

// for test use, not needed to perform DSTs

// naive dst 2 algorithm
extern int test_exec_f (
    const size_t nitems,
    const double * xs,
    double * ys
);

// naive dst 3 algorithm
extern int test_exec_b (
    const size_t nitems,
    const double * xs,
    double * ys
);

// compare two signals
extern int test_compare (
    const char message[],
    const size_t nitems,
    const double * y0s,
    const double * y1s
);

#endif // TEST_H
