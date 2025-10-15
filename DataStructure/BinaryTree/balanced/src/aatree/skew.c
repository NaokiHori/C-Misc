#include "./skew.h"

/**
 * fix left horizontal link by performing a right rotation
 * BEFORE: level(p) = level(c), left horizontal link
 *      p
 *     / \
 *    c   y
 *   / \
 *  x   g
 * AFTER: right horizontal link
 *      c
 *     / \
 *    x   p
 *       / \
 *      g   y
 */
int aatree_internal_skew(
    aatree_internal_node_t ** const parent
) {
  if (NULL == *parent) {
    return 1;
  }
  aatree_internal_node_t * const child = (*parent)->left;
  if (NULL == child) {
    return 0;
  }
  if ((*parent)->level != child->level) {
    return 0;
  }
  aatree_internal_node_t * const grandchild = child->right;
  child->right = *parent;
  (*parent)->left = grandchild;
  *parent = child;
  return 0;
}

#if defined(AATREE_SKEW_TEST)

#include <assert.h>
#include "./test_util.h"

static int test_left_horizontal_link_performs_right_rotation(
    void
) {
  const size_t items[] = {0, 1, 2, 3, 4};
  typedef struct {
    aatree_internal_node_t p;
    aatree_internal_node_t c;
    aatree_internal_node_t g;
    aatree_internal_node_t x;
    aatree_internal_node_t y;
  } nodes_t;
  nodes_t nodes = (nodes_t){
    .p = {.item = items + 0, .level = 2, .left = &nodes.c, .right = &nodes.y},
    .c = {.item = items + 1, .level = 2, .left = &nodes.x, .right = &nodes.g},
    .g = {.item = items + 2, .level = 1, .left = NULL, .right = NULL},
    .x = {.item = items + 3, .level = 1, .left = NULL, .right = NULL},
    .y = {.item = items + 4, .level = 1, .left = NULL, .right = NULL},
  };
  aatree_internal_node_t * root_node = &nodes.p;
  assert(0 == aatree_internal_skew(&root_node));
  ASSERT_NODE(root_node, &nodes.c, items + 1, 2, &nodes.x, &nodes.p);
  ASSERT_NODE(root_node->left, &nodes.x, items + 3, 1, NULL, NULL);
  ASSERT_NODE(root_node->right, &nodes.p, items + 0, 2, &nodes.g, &nodes.y);
  ASSERT_NODE(root_node->right->left, &nodes.g, items + 2, 1, NULL, NULL);
  ASSERT_NODE(root_node->right->right, &nodes.y, items + 4, 1, NULL, NULL);
  // check idempotence: performing skew on correct tree again changes nothing
  assert(0 == aatree_internal_skew(&root_node));
  ASSERT_NODE(root_node, &nodes.c, items + 1, 2, &nodes.x, &nodes.p);
  ASSERT_NODE(root_node->left, &nodes.x, items + 3, 1, NULL, NULL);
  ASSERT_NODE(root_node->right, &nodes.p, items + 0, 2, &nodes.g, &nodes.y);
  ASSERT_NODE(root_node->right->left, &nodes.g, items + 2, 1, NULL, NULL);
  ASSERT_NODE(root_node->right->right, &nodes.y, items + 4, 1, NULL, NULL);
  return 0;
}

static int test_null_parent_returns_error(
    void
) {
  aatree_internal_node_t * root_node = NULL;
  assert(1 == aatree_internal_skew(&root_node));
  return 0;
}

/**
 * p
 *  \
 *   c
 *    \
 *     g
 */
static int test_no_left_child_no_change(
    void
) {
  const size_t items[] = {0, 1, 2};
  typedef struct {
    aatree_internal_node_t p;
    aatree_internal_node_t c;
    aatree_internal_node_t g;
  } nodes_t;
  nodes_t nodes = (nodes_t){
    .p = {.item = items + 0, .level = 3, .left = NULL, .right = &nodes.c},
    .c = {.item = items + 1, .level = 2, .left = NULL, .right = &nodes.g},
    .g = {.item = items + 2, .level = 1, .left = NULL, .right = NULL},
  };
  aatree_internal_node_t * root_node = &nodes.p;
  assert(0 == aatree_internal_skew(&root_node));
  ASSERT_NODE(root_node, &nodes.p, items + 0, 3, NULL, &nodes.c);
  ASSERT_NODE(root_node->right, &nodes.c, items + 1, 2, NULL, &nodes.g);
  ASSERT_NODE(root_node->right->right, &nodes.g, items + 2, 1, NULL, NULL);
  return 0;
}

/**
 * left non-horizontal link
 *   p(2)
 *  /
 * c(1)
 */
static int test_left_child_lower_level_no_change(
    void
) {
  const size_t items[] = {0, 1};
  typedef struct {
    aatree_internal_node_t p;
    aatree_internal_node_t c;
  } nodes_t;
  nodes_t nodes = (nodes_t){
    .p = {.item = items + 0, .level = 2, .left = &nodes.c, .right = NULL},
    .c = {.item = items + 1, .level = 1, .left = NULL, .right = NULL},
  };
  aatree_internal_node_t * root_node = &nodes.p;
  assert(0 == aatree_internal_skew(&root_node));
  ASSERT_NODE(root_node, &nodes.p, items + 0, 2, &nodes.c, NULL);
  ASSERT_NODE(root_node->left, &nodes.c, items + 1, 1, NULL, NULL);
  return 0;
}

/**
 * left horizontal link
 *   p(1)
 *  /
 * c(1)
 */
static int test_minimal_two_node_rotation(
    void
) {
  const size_t items[] = {0, 1};
  typedef struct {
    aatree_internal_node_t p;
    aatree_internal_node_t c;
  } nodes_t;
  nodes_t nodes = (nodes_t){
    .p = {.item = items + 0, .level = 1, .left = &nodes.c, .right = NULL},
    .c = {.item = items + 1, .level = 1, .left = NULL, .right = NULL},
  };
  aatree_internal_node_t * root_node = &nodes.p;
  assert(0 == aatree_internal_skew(&root_node));
  ASSERT_NODE(root_node, &nodes.c, items + 1, 1, NULL, &nodes.p);
  ASSERT_NODE(root_node->right, &nodes.p, items + 0, 1, NULL, NULL);
  return 0;
}

/**
 * verify that skew correctly updates the pointer provided by the caller
 * when it operates on a subtree (double pointer behavior)
 *
 * BEFORE: skew(p) is applied
 *       r
 *      / \
 *     p(2)z
 *    / \
 *   c(2)y
 *  / \
 * x   g
 *
 * AFTER
 *       r
 *      / \
 *     c   z
 *    / \
 *   x   p
 *      / \
 *     g   y
 */
static int test_subtree_rotation_updates_parent_pointer(
    void
) {
  const size_t items[] = {0, 1, 2, 3, 4, 5, 6};
  typedef struct {
    aatree_internal_node_t r;
    aatree_internal_node_t p;
    aatree_internal_node_t c;
    aatree_internal_node_t g;
    aatree_internal_node_t x;
    aatree_internal_node_t y;
    aatree_internal_node_t z;
  } nodes_t;
  // NOTE: invalid tree, only level(c) = level(p) is focused
  nodes_t nodes = (nodes_t){
    .r = {.item = items + 5, .level = 3, .left = &nodes.p, .right = &nodes.z},
    .p = {.item = items + 3, .level = 2, .left = &nodes.c, .right = &nodes.y},
    .c = {.item = items + 1, .level = 2, .left = &nodes.x, .right = &nodes.g},
    .g = {.item = items + 2, .level = 1, .left = NULL, .right = NULL},
    .x = {.item = items + 0, .level = 1, .left = NULL, .right = NULL},
    .y = {.item = items + 4, .level = 1, .left = NULL, .right = NULL},
    .z = {.item = items + 6, .level = 1, .left = NULL, .right = NULL},
  };
  aatree_internal_node_t * root_node = &nodes.r;
  assert(0 == aatree_internal_skew(&root_node->left));
  ASSERT_NODE(root_node, &nodes.r, items + 5, 3, &nodes.c, &nodes.z);
  ASSERT_NODE(root_node->left, &nodes.c, items + 1, 2, &nodes.x, &nodes.p);
  ASSERT_NODE(root_node->right, &nodes.z, items + 6, 1, NULL, NULL);
  ASSERT_NODE(root_node->left->left, &nodes.x, items + 0, 1, NULL, NULL);
  ASSERT_NODE(root_node->left->right, &nodes.p, items + 3, 2, &nodes.g, &nodes.y);
  ASSERT_NODE(root_node->left->right->left, &nodes.g, items + 2, 1, NULL, NULL);
  ASSERT_NODE(root_node->left->right->right, &nodes.y, items + 4, 1, NULL, NULL);
  return 0;
}

int main(
    void
) {
  test_left_horizontal_link_performs_right_rotation();
  test_null_parent_returns_error();
  test_no_left_child_no_change();
  test_left_child_lower_level_no_change();
  test_minimal_two_node_rotation();
  test_subtree_rotation_updates_parent_pointer();
  return 0;
}

#endif // AATREE_SKEW_TEST_H
