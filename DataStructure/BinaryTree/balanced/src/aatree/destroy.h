#if !defined(AATREE_DESTROY_H)
#define AATREE_DESTROY_H

#include <stddef.h>
#include "./node.h"

extern int aatree_internal_destroy(
    aatree_internal_node_t * const parent
);

#endif // AATREE_DESTROY_H
