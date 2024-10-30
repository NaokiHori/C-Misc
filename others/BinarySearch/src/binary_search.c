#include <stdio.h>
#include "binary_search.h"

int binary_search (
    const void * const items,
    const int nitems,
    const size_t size,
    int (* const compare) (
      const void * const a,
      const void * const b
    ),
    const void * const target,
    int * const target_index
) {
  // locate the maximum index of an item,
  //   which is smaller than or equal to the given item "target"
  // NOTE: when the target is smaller than the smallest item in the list,
  //       -1 will be returned
  // E.g., for a list of integer values:
  //   [2, 3, 5, 8, 13]
  //   this function takes and returns the following:
  //     [INT_MIN, 1]  -> -1
  //     2             ->  0
  //     [3, 4]        ->  1
  //     [5, 7]        ->  2
  //     [8, 12]       ->  3
  //     [13, INT_MAX] ->  4
  if (0 == nitems) {
    puts("nitems should be positive");
    return 1;
  }
  // initial indices
  int bound_indices[2] = {-1, nitems};
  // continue until the bounding indices are adjacent
  while (1 < bound_indices[1] - bound_indices[0]) {
    const int probed_index = bound_indices[0] + (bound_indices[1] - bound_indices[0]) / 2;
    // pointer to the item of interest
    // NOTE: size of each element "size" = stride is given by user
    const void * const probed_item = (char *)items + size * probed_index;
    const int compared_result = compare(probed_item, target);
    if (0 < compared_result) {
      // probed item is larger than the target
      // make the successor index smaller
      bound_indices[1] = probed_index;
    } else {
      // probed item is smaller than or equal to the target
      // make the predecessor index larger
      bound_indices[0] = probed_index;
    }
  }
  *target_index = bound_indices[0];
  return 0;
}

