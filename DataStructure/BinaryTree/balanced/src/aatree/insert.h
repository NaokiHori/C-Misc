#if !defined(AATREE_INSERT_H)
#define AATREE_INSERT_H

#include "./node.h"

extern int aatree_internal_insert(
    int (* const compare)(
        const void * const a,
        const void * const b
    ),
    aatree_internal_node_t ** const parent,
    const void * const item
);

#endif // AATREE_INSERT_H
