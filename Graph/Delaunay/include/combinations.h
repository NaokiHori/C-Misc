#if !defined(COMBINATIONS_H)
#define COMBINATIONS_H

extern int init_combination (
    const size_t r,
    size_t * combination
);

extern int find_next_combination (
    const size_t n,
    const size_t r,
    size_t * combination
);

#endif // COMBINATIONS_H
