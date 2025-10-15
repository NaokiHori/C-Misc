#if !defined(AATREE_TEST_UTIL_H)
#define AATREE_TEST_UTIL_H

#define ASSERT_NODE( \
    inspected_node, \
    expected_node, \
    expected_item, \
    expected_level, \
    expected_left, \
    expected_right \
) { \
  assert(inspected_node == expected_node); \
  assert(inspected_node->item == expected_item); \
  assert(inspected_node->level == expected_level); \
  assert(inspected_node->left == expected_left); \
  assert(inspected_node->right == expected_right); \
}

#endif // AATREE_TEST_UTIL_H
