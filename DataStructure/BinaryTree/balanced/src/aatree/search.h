#if !defined(AATREE_SEARCH_H)
#define AATREE_SEARCH_H

#include <stdbool.h>
#include "./node.h"

extern int aatree_internal_search(
    int (* const compare)(
        const void * const a,
        const void * const b
    ),
    const aatree_internal_node_t * const parent,
    const void * const item,
    bool * const is_present
);

#endif // AATREE_SEARCH_H
