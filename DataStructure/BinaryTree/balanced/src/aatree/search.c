#include "./search.h"

int aatree_internal_search(
    int (* const compare)(
        const void * const a,
        const void * const b
    ),
    const aatree_internal_node_t * const parent,
    const void * const item,
    bool * const is_present
) {
  if (NULL == parent) {
    *is_present = false;
    return 0;
  }
  const int compared = compare(item, parent->item);
  if (compared < 0) {
    return aatree_internal_search(compare, parent->left, item, is_present);
  } else if (0 < compared) {
    return aatree_internal_search(compare, parent->right, item, is_present);
  } else {
    *is_present = true;
    return 0;
  }
}

#if defined(AATREE_SEARCH_TEST)

#include <assert.h>
#include "./check.h"
#include "./test_util.h"

static int compare(
    const void * const void_ptr_a,
    const void * const void_ptr_b
) {
  size_t * const a = (size_t *)void_ptr_a;
  size_t * const b = (size_t *)void_ptr_b;
  if (*a < *b) {
    return -1;
  } else if (*b < *a) {
    return 1;
  } else {
    return 0;
  }
}

static int test_search_empty_tree_returns_null(
    void
) {
  bool is_present = false;
  const size_t item = 0;
  assert(0 == aatree_internal_search(compare, NULL, &item, &is_present));
  assert(false == is_present);
  return 0;
}

static int test_search_single_node_tree_found_and_not_found(
    void
) {
  const size_t item = 0;
  aatree_internal_node_t parent = {
    .item = &item,
    .level = 1,
    .left = NULL,
    .right = NULL,
  };
  const size_t candidates[] = {0, 1};
  bool is_present = false;
  assert(0 == aatree_internal_search(compare, &parent, candidates + 0, &is_present));
  assert(true == is_present);
  assert(0 == aatree_internal_search(compare, &parent, candidates + 1, &is_present));
  assert(false == is_present);
  return 0;
}

static int test_search_finds_correct_item(
    void
) {
  const size_t items[] = {0, 1, 2, 3, 4, 5, 6};
  aatree_internal_node_t nodes[7] = {
    {.item = items + 3, .level = 3, .left = nodes + 1, .right = nodes + 2},
    {.item = items + 1, .level = 2, .left = nodes + 3, .right = nodes + 4},
    {.item = items + 5, .level = 2, .left = nodes + 5, .right = nodes + 6},
    {.item = items + 0, .level = 1, .left = NULL, .right = NULL},
    {.item = items + 2, .level = 1, .left = NULL, .right = NULL},
    {.item = items + 4, .level = 1, .left = NULL, .right = NULL},
    {.item = items + 6, .level = 1, .left = NULL, .right = NULL},
  };
  bool is_present = false;
  for (size_t i = 0; i < 7; i++) {
    assert(0 == aatree_internal_search(compare, nodes, &i, &is_present));
    assert(true == is_present);
  }
  for (size_t i = 7; i < 14; i++) {
    assert(0 == aatree_internal_search(compare, nodes, &i, &is_present));
    assert(false == is_present);
  }
  // check the tree is not altered
  aatree_internal_node_t * root = nodes;
  ASSERT_NODE(root, nodes + 0, items + 3, 3, nodes + 1, nodes + 2);
  ASSERT_NODE(root->left, nodes + 1, items + 1, 2, nodes + 3, nodes + 4);
  ASSERT_NODE(root->right, nodes + 2, items + 5, 2, nodes + 5, nodes + 6);
  ASSERT_NODE(root->left->left, nodes + 3, items + 0, 1, NULL, NULL);
  ASSERT_NODE(root->left->right, nodes + 4, items + 2, 1, NULL, NULL);
  ASSERT_NODE(root->right->left, nodes + 5, items + 4, 1, NULL, NULL);
  ASSERT_NODE(root->right->right, nodes + 6, items + 6, 1, NULL, NULL);
  return 0;
}

int main(
    void
) {
  test_search_empty_tree_returns_null();
  test_search_single_node_tree_found_and_not_found();
  test_search_finds_correct_item();
  return 0;
}

#endif // AATREE_SEARCH_TEST
