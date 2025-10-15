#include <stdlib.h>
#include "./destroy.h"

int aatree_internal_destroy(
    aatree_internal_node_t * const parent
) {
  if (NULL == parent) {
    return 0;
  }
  aatree_internal_node_t * const left = parent->left;
  aatree_internal_node_t * const right = parent->right;
  if (NULL != left) {
    aatree_internal_destroy(left);
  }
  if (NULL != right) {
    aatree_internal_destroy(right);
  }
  free(parent);
  return 0;
}

