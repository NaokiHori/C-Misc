#if !defined(PREDICT_H)
#define PREDICT_H

#include <stddef.h> // size_t
#include "types.h" // dataset_t, vector_t

extern int predict(
    const size_t nitems,
    const dataset_t * const items,
    const vector_t * const weights
);

#endif // PREDICT_H
