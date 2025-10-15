#if !defined(AATREE_DELETE_H)
#define AATREE_DELETE_H

#include <stddef.h>
#include "./node.h"

extern int aatree_internal_delete(
    int (* const compare)(
        const void * const a,
        const void * const b
    ),
    aatree_internal_node_t ** const parent,
    const void * const item
);

#endif // AATREE_DELETE_H
