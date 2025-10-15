#include <stdio.h>
#include "./check.h"

int aatree_internal_check(
    const aatree_internal_node_t * const parent
) {
  // https://en.wikipedia.org/wiki/AA_tree#Balancing_rotations
  // - The level of every leaf node is one.
  //   - level(leaf) = 1
  // - The level of every left child is exactly one less than that of its parent.
  //   - level(left) = level(parent) - 1
  // - The level of every right child is equal to or one less than that of its parent.
  //   - level(right) = level(parent) or level(right) = level(parent) - 1
  // - The level of every right grandchild is strictly less than that of its grandparent.
  //   - level(right grandchild) < level(parent)
  // - Every node of level greater than one has two children.
  if (NULL == parent) {
    return 0;
  }
  const aatree_internal_node_t * const left = parent->left;
  const aatree_internal_node_t * const right = parent->right;
  const aatree_internal_node_t * const grandchild = NULL == right ? NULL : right->right;
  const size_t p_level = parent->level;
  const size_t l_level = NULL == left ? 0 : left->level;
  const size_t r_level = NULL == right ? 0 : right->level;
  const size_t g_level = NULL == grandchild ? 0 : grandchild->level;
  // leaf node's level should be 1
  if (NULL == left && NULL == right) {
    if (1 != p_level) {
      printf("leaf node level %zu should be 1\n", p_level);
      return 1;
    }
  }
  // check parent's level and left child's level
  if (NULL != left) {
    if (p_level != l_level + 1) {
      printf("parent level %zu should be left child level %zu + 1\n", p_level, l_level);
      return 1;
    }
    // check tree under left child
    if (0 != aatree_internal_check(left)) {
      return 1;
    }
  }
  // check parent's level and right child's level
  if (NULL != right) {
    if (p_level != r_level && p_level != r_level + 1) {
      printf("parent level %zu should be right child level %zu or + 1\n", p_level, r_level);
      return 1;
    }
    // check tree under right child
    if (0 != aatree_internal_check(right)) {
      return 1;
    }
  }
  // check parent's level and right grandchild's level
  if (NULL != grandchild) {
    if (p_level <= g_level) {
      printf("parent level %zu should be less than or equal to right grandchild level %zu\n", p_level, g_level);
      return 1;
    }
  }
  // check number of children
  if (1 < p_level) {
    if (NULL == left || NULL == right) {
      printf("parent level %zu should have two children\n", p_level);
      return 1;
    }
  }
  return 0;
}

