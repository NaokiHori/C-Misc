#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include "./display.h"

static const size_t indent = 8;

static int display(
    int (* const print)(
        const void * item
    ),
    const aatree_internal_node_t * const parent,
    const size_t switch_flags,
    const size_t depth
) {
  if (NULL == parent) {
    return 0;
  }
  // right
  // set 1 at depth's bit
  if (0 != display(print, parent->right, (1 << depth) | switch_flags, depth + 1)) {
    return 1;
  }
  // center
  if (0 < depth) {
    for (size_t j = 0; j < depth - 1; j++) {
      // draw a vertical line if it swaps branch, i.e.,
      // - right -> left
      // - left -> right
      // leave it if the direction is consistent, i.e.,
      // - right -> right
      // - left -> left
      const bool is_connected =
        ((switch_flags >> j) & 1)
        ^
        ((switch_flags >> (j + 1)) & 1);
      for (size_t i = 0; i < indent; i++) {
        printf("%c", 0 == i % indent && is_connected ? '|' : ' ');
      }
    }
    for (size_t i = 0; i < indent; i++) {
      printf("%c", 0 == i ? '+' : '-');
    }
  }
  print(parent->item);
  printf("(%zu)\n", parent->level);
  // left
  // set 0 at depth's bit
  if (0 != display(print, parent->left, (~(1 << depth)) & switch_flags, depth + 1)) {
    return 1;
  }
  return 0;
}

int aatree_internal_display(
    int (* const print)(
        const void * item
    ),
    const aatree_internal_node_t * const parent
) {
  if (0 != display(print, parent, 0, 0)) {
    return 1;
  }
  return 0;
}

