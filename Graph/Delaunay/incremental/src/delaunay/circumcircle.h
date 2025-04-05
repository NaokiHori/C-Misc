#if !defined(CIRCUMCIRCLE_H)
#define CIRCUMCIRCLE_H

#include <stdbool.h>
#include "delaunay.h"

extern bool point_is_in_circumcircle(
    const point_t * const point,
    const triangle_t * const triangle
);

#endif // CIRCUMCIRCLE_H
