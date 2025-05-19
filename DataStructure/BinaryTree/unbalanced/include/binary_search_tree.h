#if !defined(BINARY_SEARCH_TREE_H)
#define BINARY_SEARCH_TREE_H

#include <stddef.h> // size_t

typedef struct binary_search_tree_t binary_search_tree_t;

extern int binary_search_tree_init(
    binary_search_tree_t ** const binary_search_tree
);

extern int binary_search_tree_insert(
    binary_search_tree_t * const binary_search_tree,
    const size_t item
);

extern int binary_search_tree_delete(
    binary_search_tree_t * const binary_search_tree,
    const size_t item
);

extern int binary_search_tree_search(
    const binary_search_tree_t * const binary_search_tree,
    const size_t item
);

extern int binary_search_tree_finalize(
    binary_search_tree_t ** const binary_search_tree
);

#endif // BINARY_SEARCH_TREE_H
