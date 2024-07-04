#if !defined(INIT_H)
#define INIT_H

#include <stddef.h>
#include "dijkstra.h"

extern int init_graph (
    const size_t nnodes,
    size_t * nlinks,
    link_t ** links
);

#endif // INIT_H
