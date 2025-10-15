#if !defined(DISPLAY_H)
#define DISPLAY_H

#include "./node.h"

extern int aatree_internal_display(
    int (* const print)(
        const void * item
    ),
    const aatree_internal_node_t * const parent
);

#endif // DISPLAY_H
