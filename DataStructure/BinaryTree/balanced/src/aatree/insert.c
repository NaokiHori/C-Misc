#include <stdlib.h>
#include "./insert.h"
#include "./skew.h"
#include "./split.h"

static int create(
    aatree_internal_node_t ** const node,
    const void * const item
) {
  *node = malloc(sizeof(aatree_internal_node_t));
  if (NULL == *node) {
    return 1;
  }
  (*node)->left = NULL;
  (*node)->right = NULL;
  (*node)->item = item;
  // leaf node
  (*node)->level = 1;
  return 0;
}

int aatree_internal_insert(
    int (* const compare)(
        const void * const a,
        const void * const b
    ),
    aatree_internal_node_t ** const parent,
    const void * const item
) {
  if (NULL == *parent) {
    return create(parent, item);
  }
  const int compared = compare(item, (*parent)->item);
  if (compared < 0) {
    // insert to left
    if (0 != aatree_internal_insert(compare, &(*parent)->left, item)) {
      return 1;
    }
  } else if (0 < compared) {
    // insert to right
    if (0 != aatree_internal_insert(compare, &(*parent)->right, item)) {
      return 1;
    }
  } else {
    // duplicated entry, treat as error for now
    return 1;
  }
  // rebalance
  if (0 != aatree_internal_skew(parent)) {
    return 1;
  }
  if (0 != aatree_internal_split(parent)) {
    return 1;
  }
  return 0;
}

#if defined(AATREE_INSERT_TEST)

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

static int test_insert_into_empty_tree_creates_root(
    void
) {
  const size_t item = 0;
  aatree_internal_node_t * root = NULL;
  assert(0 == aatree_internal_insert(compare, &root, &item));
  assert(0 == aatree_internal_check(root));
  ASSERT_NODE(root, root, &item, 1, NULL, NULL);
  return 0;
}

static int test_insert_three_increasing_items_performs_split(
    void
) {
  const size_t items[] = {0, 1, 2};
  aatree_internal_node_t * root = NULL;
  assert(0 == aatree_internal_insert(compare, &root, items + 0));
  assert(0 == aatree_internal_check(root));
  assert(0 == aatree_internal_insert(compare, &root, items + 1));
  assert(0 == aatree_internal_check(root));
  assert(0 == aatree_internal_insert(compare, &root, items + 2));
  assert(0 == aatree_internal_check(root));
  ASSERT_NODE(root, root, items + 1, 2, root->left, root->right);
  ASSERT_NODE(root->left, root->left, items + 0, 1, NULL, NULL);
  ASSERT_NODE(root->right, root->right, items + 2, 1, NULL, NULL);
  return 0;
}

static int test_insert_three_decreasing_items_performs_skew_and_split(
    void
) {
  const size_t items[] = {0, 1, 2};
  aatree_internal_node_t * root = NULL;
  assert(0 == aatree_internal_insert(compare, &root, items + 2));
  assert(0 == aatree_internal_check(root));
  assert(0 == aatree_internal_insert(compare, &root, items + 1));
  assert(0 == aatree_internal_check(root));
  assert(0 == aatree_internal_insert(compare, &root, items + 0));
  assert(0 == aatree_internal_check(root));
  ASSERT_NODE(root, root, items + 1, 2, root->left, root->right);
  ASSERT_NODE(root->left, root->left, items + 0, 1, NULL, NULL);
  ASSERT_NODE(root->right, root->right, items + 2, 1, NULL, NULL);
  return 0;
}

static int test_insert_items_in_ascending_order_produces_balanced_levels(
    void
) {
  const size_t items[] = {0, 1, 2, 3, 4, 5, 6, 7};
  aatree_internal_node_t * root = NULL;
  for (size_t i = 0; i < sizeof(items) / sizeof(items[0]); i++) {
    assert(0 == aatree_internal_insert(compare, &root, items + i));
    assert(0 == aatree_internal_check(root));
  }
  ASSERT_NODE(root, root, items + 3, 3, root->left, root->right);
  ASSERT_NODE(root->left, root->left, items + 1, 2, root->left->left, root->left->right);
  ASSERT_NODE(root->right, root->right, items + 5, 2, root->right->left, root->right->right);
  ASSERT_NODE(root->left->left, root->left->left, items + 0, 1, NULL, NULL);
  ASSERT_NODE(root->left->right, root->left->right, items + 2, 1, NULL, NULL);
  ASSERT_NODE(root->right->left, root->right->left, items + 4, 1, NULL, NULL);
  ASSERT_NODE(root->right->right, root->right->right, items + 6, 1, NULL, root->right->right->right);
  ASSERT_NODE(root->right->right->right, root->right->right->right, items + 7, 1, NULL, NULL);
  return 0;
}

static int test_insert_duplicate_returns_error_and_tree_unchanged(
    void
) {
  const size_t items[] = {0, 1};
  aatree_internal_node_t * root = NULL;
  assert(0 == aatree_internal_insert(compare, &root, items + 0));
  assert(0 == aatree_internal_check(root));
  assert(0 == aatree_internal_insert(compare, &root, items + 1));
  assert(0 == aatree_internal_check(root));
  ASSERT_NODE(root, root, items + 0, 1, NULL, root->right);
  ASSERT_NODE(root->right, root->right, items + 1, 1, NULL, NULL);
  assert(1 == aatree_internal_insert(compare, &root, items + 1));
  assert(0 == aatree_internal_check(root));
  ASSERT_NODE(root, root, items + 0, 1, NULL, root->right);
  ASSERT_NODE(root->right, root->right, items + 1, 1, NULL, NULL);
  return 0;
}

int main(
    void
) {
  test_insert_into_empty_tree_creates_root();
  test_insert_three_increasing_items_performs_split();
  test_insert_three_decreasing_items_performs_skew_and_split();
  test_insert_items_in_ascending_order_produces_balanced_levels();
  test_insert_duplicate_returns_error_and_tree_unchanged();
  return 0;
}

#endif // AATREE_INSERT_TEST
