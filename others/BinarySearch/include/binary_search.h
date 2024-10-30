#if !defined(BINARY_SEARCH_H)
#define BINARY_SEARCH_H

#include <stddef.h> // size_t

/**
 * @brief From a list of items: "items", identifying the position of the given item "target"
 *        by means of the binary search algorithm (approximate match)
 * @param[in]  items   Pointer to the list of items
 * @param[in]  nitems  Number of "items"
 * @param[in]  size    Size of each item (in bytes)
 * @param[in]  compare Pointer to a function which specifies the order of two items (c.f. qsort(3))
 * @param[in]  target  Item to be identified in the list
 * @param[out] index   Index of "target"
 * @return error code (success: 0 or failure: otherwise)
 */
extern int binary_search (
    const void * const items,
    const int nitems,
    const size_t size,
    int (* const compare) (
      const void * const a,
      const void * const b
    ),
    const void * const target,
    int * const target_index
);

#endif // BINARY_SEARCH_H
