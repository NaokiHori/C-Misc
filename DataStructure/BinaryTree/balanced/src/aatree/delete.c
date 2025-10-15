#include <stdlib.h>
#include "./delete.h"
#include "./skew.h"
#include "./split.h"

static int decrease_level(
    aatree_internal_node_t * const parent
) {
  aatree_internal_node_t * const left = parent->left;
  aatree_internal_node_t * const right = parent->right;
  const size_t l_level = NULL == left ? 0 : left->level;
  const size_t r_level = NULL == right ? 0 : right->level;
  const size_t expected_level = 1 + (l_level < r_level ? l_level : r_level);
  if (parent->level <= expected_level) {
    return 0;
  }
  parent->level = expected_level;
  if (r_level <= expected_level) {
    return 0;
  }
  right->level = expected_level;
  return 0;
}

static int rebalance_tree(
    aatree_internal_node_t ** const parent
) {
  if (NULL == *parent) {
    return 0;
  }
  // rebalance tree
  if (0 != decrease_level(*parent)) {
    return 1;
  }
  // three skew operations
  if (0 != aatree_internal_skew(parent)) {
    return 1;
  }
  if (NULL != (*parent)->right) {
    if (0 != aatree_internal_skew(&(*parent)->right)) {
      return 1;
    }
  }
  if (NULL != (*parent)->right && NULL != (*parent)->right->right) {
    if (0 != aatree_internal_skew(&(*parent)->right->right)) {
      return 1;
    }
  }
  // two split operations
  if (0 != aatree_internal_split(parent)) {
    return 1;
  }
  if (NULL != (*parent)->right) {
    if (0 != aatree_internal_split(&(*parent)->right)) {
      return 1;
    }
  }
  return 0;
}

int aatree_internal_delete(
    int (* const compare)(
        const void * const a,
        const void * const b
    ),
    aatree_internal_node_t ** const parent,
    const void * const item
) {
  if (NULL == *parent) {
    return 0;
  }
  // look for the node storig the desired item recursively
  const int compared = compare(item, (*parent)->item);
  if (compared < 0) {
    if (0 != aatree_internal_delete(compare, &(*parent)->left, item)) {
      return 1;
    }
  } else if (0 < compared) {
    if (0 != aatree_internal_delete(compare, &(*parent)->right, item)) {
      return 1;
    }
  } else {
    // the parent node is subject to deletion
    // there are several scenarios that needs to be dealt with
    aatree_internal_node_t * const left = (*parent)->left;
    aatree_internal_node_t * const right = (*parent)->right;
    if (NULL == left || NULL == right) {
      // the parent node has at most one child
      // remove it and replace it with the child
      free(*parent);
      *parent = NULL == left ? right : left;
    } else {
      // the parent node has two children
      // look for successor
      aatree_internal_node_t * successor = (*parent)->right;
      while (NULL != successor->left) {
        successor = successor->left;
      }
      (*parent)->item = successor->item;
      if (0 != aatree_internal_delete(compare, &(*parent)->right, successor->item)) {
        return 1;
      }
    }
  }
  return rebalance_tree(parent);
}

#if defined(AATREE_DELETE_TEST)

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

static int setup_node(
    aatree_internal_node_t * const node,
    const size_t * const item,
    const size_t level,
    aatree_internal_node_t * const left,
    aatree_internal_node_t * const right
) {
  node->item = item;
  node->level = level;
  node->left = left;
  node->right = right;
  return 0;
}

static int test_delete_from_empty_tree_noop(
    void
) {
  const size_t item = 0;
  aatree_internal_node_t * root = NULL;
  assert(0 == aatree_internal_delete(compare, &root, &item));
  assert(0 == aatree_internal_check(root));
  assert(root == NULL);
  return 0;
}

static int test_delete_single_root_results_in_empty_tree(
    void
) {
  const size_t item = 0;
  aatree_internal_node_t * root = malloc(sizeof(aatree_internal_node_t));
  setup_node(root, &item, 1, NULL, NULL);
  assert(0 == aatree_internal_check(root));
  ASSERT_NODE(root, root, &item, 1, NULL, NULL);
  assert(0 == aatree_internal_delete(compare, &root, &item));
  assert(0 == aatree_internal_check(root));
  assert(root == NULL);
  return 0;
}

/**
 * BEFORE:
 *     1(2)
 *    / \
 *  0(1) 2(1)
 * AFTER:
 *     1(1)
 *      \
 *       2(1)
 */
static int test_delete_leaf_node_removes_only_that_node(
    void
) {
  const size_t items[] = {0, 1, 2};
  aatree_internal_node_t * parent = malloc(sizeof(aatree_internal_node_t));
  aatree_internal_node_t * left = malloc(sizeof(aatree_internal_node_t));
  aatree_internal_node_t * right = malloc(sizeof(aatree_internal_node_t));
  setup_node(parent, items + 1, 2, left, right);
  setup_node(left, items + 0, 1, NULL, NULL);
  setup_node(right, items + 2, 1, NULL, NULL);
  assert(0 == aatree_internal_check(parent));
  aatree_internal_node_t * root = parent;
  assert(0 == aatree_internal_delete(compare, &parent, items + 0));
  assert(0 == aatree_internal_check(parent));
  // parent level is reduced to match the right child's level
  ASSERT_NODE(parent, root, items + 1, 1, NULL, right);
  ASSERT_NODE(parent->right, right, items + 2, 1, NULL, NULL);
  free(parent->right);
  free(parent);
  return 0;
}

/**
 * BEFORE:
 * 0(1)
 *  \
 *   1(1)
 * AFTER:
 * 1(1)
 */
static int test_delete_node_with_single_child_promotes_child(
    void
) {
  const size_t items[] = {0, 1};
  aatree_internal_node_t * parent = malloc(sizeof(aatree_internal_node_t));
  aatree_internal_node_t * right = malloc(sizeof(aatree_internal_node_t));
  setup_node(parent, items + 0, 1, NULL, right);
  setup_node(right, items + 1, 1, NULL, NULL);
  assert(0 == aatree_internal_check(parent));
  assert(0 == aatree_internal_delete(compare, &parent, items + 0));
  assert(0 == aatree_internal_check(parent));
  ASSERT_NODE(parent, right, items + 1, 1, NULL, NULL);
  free(parent);
  return 0;
}

/**
 * BEFORE:
 *     1(2)
 *    / \
 *  0(1) 2(1)
 * AFTER:
 *     0(1)
 *      \
 *       2(1)
 * the resulting tree is obtained as follows:
 * 1. after 1 is deleted, successor (2) is promoted
 * 2. level of the new root node is reduced
 * 3. the tree is skewed due to left horizontal link
 */
static int test_delete_node_with_two_children_uses_inorder_successor(
    void
) {
  const size_t items[] = {0, 1, 2};
  aatree_internal_node_t * parent = malloc(sizeof(aatree_internal_node_t));
  aatree_internal_node_t * left = malloc(sizeof(aatree_internal_node_t));
  aatree_internal_node_t * right = malloc(sizeof(aatree_internal_node_t));
  setup_node(parent, items + 1, 2, left, right);
  setup_node(left, items + 0, 1, NULL, NULL);
  setup_node(right, items + 2, 1, NULL, NULL);
  assert(0 == aatree_internal_check(parent));
  aatree_internal_node_t * root = parent;
  assert(0 == aatree_internal_delete(compare, &parent, items + 1));
  assert(0 == aatree_internal_check(parent));
  ASSERT_NODE(parent, left, items + 0, 1, NULL, root);
  ASSERT_NODE(parent->right, root, items + 2, 1, NULL, NULL);
  free(parent->right);
  free(parent);
  return 0;
}

/**
 * BEFORE:
 *        3(3)
 *      /     \
 *    1(2)     5(2)
 *   / \       / \
 * 0(1) 2(1) 4(1) 6(1)
 * checking that deletions of 0 and 6 do not violate AA requirements
 *
 * AFTER DELETION OF 0:
 *        3(2)
 *      /     \
 *    1(1)     5(2)
 *     \       / \
 *      2(1) 4(1) 6(1)
 *
 * AFTER DELETION OF 6:
 *        3(2)
 *      /     \
 *    1(1)     4(1)
 *     \        \
 *      2(1)     5(1)
 */
static int test_delete_minimum_and_maximum_preserves_balance(
    void
) {
  const size_t items[] = {0, 1, 2, 3, 4, 5, 6};
  aatree_internal_node_t * parent = malloc(sizeof(aatree_internal_node_t));
  aatree_internal_node_t * children[] = {
    malloc(sizeof(aatree_internal_node_t)),
    malloc(sizeof(aatree_internal_node_t)),
  };
  aatree_internal_node_t * grandchildren[] = {
    malloc(sizeof(aatree_internal_node_t)),
    malloc(sizeof(aatree_internal_node_t)),
    malloc(sizeof(aatree_internal_node_t)),
    malloc(sizeof(aatree_internal_node_t)),
  };
  setup_node(parent, items + 3, 3, children[0], children[1]);
  setup_node(children[0], items + 1, 2, grandchildren[0], grandchildren[1]);
  setup_node(children[1], items + 5, 2, grandchildren[2], grandchildren[3]);
  setup_node(grandchildren[0], items + 0, 1, NULL, NULL);
  setup_node(grandchildren[1], items + 2, 1, NULL, NULL);
  setup_node(grandchildren[2], items + 4, 1, NULL, NULL);
  setup_node(grandchildren[3], items + 6, 1, NULL, NULL);
  assert(0 == aatree_internal_check(parent));
  // delete minimam
  assert(0 == aatree_internal_delete(compare, &parent, items + 0));
  assert(0 == aatree_internal_check(parent));
  ASSERT_NODE(parent, parent, items + 3, 2, children[0], children[1]);
  ASSERT_NODE(parent->left, children[0], items + 1, 1, NULL, grandchildren[1]);
  ASSERT_NODE(parent->right, children[1], items + 5, 2, grandchildren[2], grandchildren[3]);
  ASSERT_NODE(parent->left->right, grandchildren[1], items + 2, 1, NULL, NULL);
  ASSERT_NODE(parent->right->left, grandchildren[2], items + 4, 1, NULL, NULL);
  ASSERT_NODE(parent->right->right, grandchildren[3], items + 6, 1, NULL, NULL);
  // delete maximum
  assert(0 == aatree_internal_delete(compare, &parent, items + 6));
  assert(0 == aatree_internal_check(parent));
  ASSERT_NODE(parent, parent, items + 3, 2, children[0], grandchildren[2]);
  ASSERT_NODE(parent->left, children[0], items + 1, 1, NULL, grandchildren[1]);
  ASSERT_NODE(parent->right, grandchildren[2], items + 4, 1, NULL, children[1]);
  ASSERT_NODE(parent->left->right, grandchildren[1], items + 2, 1, NULL, NULL);
  ASSERT_NODE(parent->right->right, children[1], items + 5, 1, NULL, NULL);
  // clean-up
  free(parent->left->right);
  free(parent->left);
  free(parent->right->right);
  free(parent->right);
  free(parent);
  return 0;
}

/**
 *    1(2)
 *   / \
 * 0(1) 2(1)
 */
static int test_delete_nonexistent_item_noop(
    void
) {
  const size_t items[] = {0, 1, 2, 3};
  aatree_internal_node_t * parent = malloc(sizeof(aatree_internal_node_t));
  aatree_internal_node_t * left = malloc(sizeof(aatree_internal_node_t));
  aatree_internal_node_t * right = malloc(sizeof(aatree_internal_node_t));
  setup_node(parent, items + 1, 2, left, right);
  setup_node(left, items + 0, 1, NULL, NULL);
  setup_node(right, items + 2, 1, NULL, NULL);
  assert(0 == aatree_internal_check(parent));
  aatree_internal_node_t * root = parent;
  assert(0 == aatree_internal_delete(compare, &parent, items + 3));
  assert(0 == aatree_internal_check(parent));
  ASSERT_NODE(parent, root, items + 1, 2, left, right);
  ASSERT_NODE(parent->left, left, items + 0, 1, NULL, NULL);
  ASSERT_NODE(parent->right, right, items + 2, 1, NULL, NULL);
  free(parent->left);
  free(parent->right);
  free(parent);
  return 0;
}

int main(
    void
) {
  test_delete_from_empty_tree_noop();
  test_delete_single_root_results_in_empty_tree();
  test_delete_leaf_node_removes_only_that_node();
  test_delete_node_with_single_child_promotes_child();
  test_delete_node_with_two_children_uses_inorder_successor();
  test_delete_minimum_and_maximum_preserves_balance();
  test_delete_nonexistent_item_noop();
  return 0;
}

#endif // AATREE_DELETE_TEST
