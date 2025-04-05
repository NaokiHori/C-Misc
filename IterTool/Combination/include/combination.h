#if !defined(COMBINATION_H)
#define COMBINATION_H

#include <stddef.h>

extern int init_combination(
    const size_t r,
    size_t * const combination
);

extern int find_next_combination(
    const size_t n,
    const size_t r,
    size_t * const combination
);

#endif // COMBINATION_H
