#if !defined(AATREE_H)
#define AATREE_H

#include <stdbool.h>

typedef struct aatree_t aatree_t;

/**
 * initialize an AA tree
 */
extern int aatree_init(
    int (* const compare)(
        const void * const a,
        const void * const b
    ),
    aatree_t ** const aatree
);

/**
 * clean-up the AA tree
 */
extern int aatree_finalize(
    aatree_t ** const aatree
);

/**
 * insert an item to the AA tree
 */
extern int aatree_insert(
    aatree_t * const aatree,
    const void * const item
);

/**
 * delete an item from the AA tree
 */
extern int aatree_delete(
    aatree_t * const aatree,
    const void * const item
);

/**
 * find an item from the AA tree
 */
extern int aatree_search(
    const aatree_t * const aatree,
    const void * const item,
    bool * const is_present
);

/**
 * display tree as an ascii art
 */
extern int aatree_display(
    int (* const print)(
        const void * item
    ),
    const aatree_t * const aatree
);

#endif // AATREE_H
