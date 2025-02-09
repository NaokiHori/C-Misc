#if !defined(LINE_SEGMENT_H)
#define LINE_SEGMENT_H

#include <stddef.h>
#include "vector.h"

extern int get_line_segment(
    const vector_t * const weights,
    size_t * const nitems,
    vector_t * const intersections
);

#endif // LINE_SEGMENT_H
