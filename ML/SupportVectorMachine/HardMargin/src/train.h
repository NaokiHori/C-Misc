#if !defined(TRAIN_H)
#define TRAIN_H

#include <stddef.h> // size_t
#include "types.h" // dataset_t, vector_t

extern int train(
    const size_t nitems,
    dataset_t * const items,
    vector_t * const weights
);

#endif // TRAIN_H
