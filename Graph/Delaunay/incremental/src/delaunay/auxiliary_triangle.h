#if !defined(AUXILIARY_TRIANGLE_H)

#include <stddef.h>
#include "delaunay.h"

extern int get_auxiliary_triangle(
    const size_t num_points,
    const point_t * const points,
    point_t * const vertices,
    triangle_node_t ** const triangle_list_head
);

extern int remove_auxiliary_triangles(
    const point_t * const vertices,
    triangle_node_t ** triangle_node
);

#endif // AUXILIARY_TRIANGLE_H
