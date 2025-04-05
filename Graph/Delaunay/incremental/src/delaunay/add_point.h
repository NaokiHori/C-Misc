#if !defined(ADD_POINT_H)
#define ADD_POINT_H

#include "delaunay.h"

extern int add_point(
    const point_t * const point,
    triangle_node_t ** const triangle_list_head
);

#endif // ADD_POINT_H
