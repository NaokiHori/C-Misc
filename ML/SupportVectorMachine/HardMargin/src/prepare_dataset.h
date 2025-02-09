#if !defined(PREPARE_DATASET_H)
#define PREPARE_DATASET_H

#include <stddef.h> // size_t
#include "types.h" // dataset_t

extern int prepare_dataset(
    const size_t nitems,
    dataset_t * const items
);

#endif // PREPARE_DATASET_H
