#include "./split.h"

/**
 * fix double right horizontal links by performing a left rotation
 * BEFORE: level(p) = level(c) = level(g), double right horizontal links
 *      p
 *     / \
 *    x   c
 *       / \
 *      y   g
 * AFTER: level(c) += 1, no horizontal link
 *      c
 *     / \
 *    p   g
 *   / \
 *  x   y
 */
int aatree_internal_split(
    aatree_internal_node_t ** const parent
) {
  if (NULL == *parent) {
    return 1;
  }
  aatree_internal_node_t * const child = (*parent)->right;
  if (NULL == child) {
    return 0;
  }
  aatree_internal_node_t * const grandchild = child->right;
  if (NULL == grandchild) {
    return 0;
  }
  if ((*parent)->level != grandchild->level) {
    return 0;
  }
  (*parent)->right = child->left;
  child->left = *parent;
  *parent = child;
  (*parent)->level += 1;
  return 0;
}

#if defined(AATREE_SPLIT_TEST)

#include <assert.h>
#include "./test_util.h"

static int test_double_right_horizontal_links_performs_left_rotation(
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
    .p = {.item = items + 0, .level = 1, .left = &nodes.x, .right = &nodes.c},
    .c = {.item = items + 1, .level = 1, .left = &nodes.y, .right = &nodes.g},
    .g = {.item = items + 2, .level = 1, .left = NULL, .right = NULL},
    .x = {.item = items + 3, .level = 1, .left = NULL, .right = NULL},
    .y = {.item = items + 4, .level = 1, .left = NULL, .right = NULL},
  };
  aatree_internal_node_t * root_node = &nodes.p;
  assert(0 == aatree_internal_split(&root_node));
  ASSERT_NODE(root_node, &nodes.c, items + 1, 2, &nodes.p, &nodes.g);
  ASSERT_NODE(root_node->left, &nodes.p, items + 0, 1, &nodes.x, &nodes.y);
  ASSERT_NODE(root_node->right, &nodes.g, items + 2, 1, NULL, NULL);
  ASSERT_NODE(root_node->left->left, &nodes.x, items + 3, 1, NULL, NULL);
  ASSERT_NODE(root_node->left->right, &nodes.y, items + 4, 1, NULL, NULL);
  // check idempotence: performing split on correct tree again changes nothing
  assert(0 == aatree_internal_split(&root_node));
  ASSERT_NODE(root_node, &nodes.c, items + 1, 2, &nodes.p, &nodes.g);
  ASSERT_NODE(root_node->left, &nodes.p, items + 0, 1, &nodes.x, &nodes.y);
  ASSERT_NODE(root_node->right, &nodes.g, items + 2, 1, NULL, NULL);
  ASSERT_NODE(root_node->left->left, &nodes.x, items + 3, 1, NULL, NULL);
  ASSERT_NODE(root_node->left->right, &nodes.y, items + 4, 1, NULL, NULL);
  return 0;
}

static int test_null_parent_returns_error(
    void
) {
  aatree_internal_node_t * root_node = NULL;
  assert(1 == aatree_internal_split(&root_node));
  return 0;
}

/**
 *   p
 *  /
 * x
 */
static int test_no_right_child_no_change(
    void
) {
  const size_t items[] = {0, 1};
  typedef struct {
    aatree_internal_node_t p;
    aatree_internal_node_t x;
  } nodes_t;
  nodes_t nodes = (nodes_t){
    .p = {.item = items + 1, .level = 2, .left = &nodes.x, .right = NULL},
    .x = {.item = items + 0, .level = 1, .left = NULL, .right = NULL},
  };
  aatree_internal_node_t * root_node = &nodes.p;
  assert(0 == aatree_internal_split(&root_node));
  ASSERT_NODE(root_node, &nodes.p, items + 1, 2, &nodes.x, NULL);
  ASSERT_NODE(root_node->left, &nodes.x, items + 0, 1, NULL, NULL);
  return 0;
}

/**
 *   p
 *  / \
 * x   c
 */
static int test_no_right_grandchild_no_change(
    void
) {
  const size_t items[] = {0, 1, 2};
  typedef struct {
    aatree_internal_node_t p;
    aatree_internal_node_t c;
    aatree_internal_node_t x;
  } nodes_t;
  nodes_t nodes = (nodes_t){
    .p = {.item = items + 1, .level = 2, .left = &nodes.x, .right = &nodes.c},
    .c = {.item = items + 2, .level = 1, .left = NULL, .right = NULL},
    .x = {.item = items + 0, .level = 1, .left = NULL, .right = NULL},
  };
  aatree_internal_node_t * root_node = &nodes.p;
  assert(0 == aatree_internal_split(&root_node));
  ASSERT_NODE(root_node, &nodes.p, items + 1, 2, &nodes.x, &nodes.c);
  ASSERT_NODE(root_node->left, &nodes.x, items + 0, 1, NULL, NULL);
  ASSERT_NODE(root_node->right, &nodes.c, items + 2, 1, NULL, NULL);
  return 0;
}

/**
 *   p(2)
 *  / \
 * x   c(1)
 *      \
 *       g(1)
 */
static int test_right_grandchild_lower_level_no_change(
    void
) {
  const size_t items[] = {0, 1, 2, 3};
  typedef struct {
    aatree_internal_node_t p;
    aatree_internal_node_t c;
    aatree_internal_node_t g;
    aatree_internal_node_t x;
  } nodes_t;
  nodes_t nodes = (nodes_t){
    .p = {.item = items + 1, .level = 2, .left = &nodes.x, .right = &nodes.c},
    .c = {.item = items + 2, .level = 1, .left = NULL, .right = &nodes.g},
    .g = {.item = items + 3, .level = 1, .left = NULL, .right = NULL},
    .x = {.item = items + 0, .level = 1, .left = NULL, .right = NULL},
  };
  aatree_internal_node_t * root_node = &nodes.p;
  assert(0 == aatree_internal_split(&root_node));
  ASSERT_NODE(root_node, &nodes.p, items + 1, 2, &nodes.x, &nodes.c);
  ASSERT_NODE(root_node->left, &nodes.x, items + 0, 1, NULL, NULL);
  ASSERT_NODE(root_node->right, &nodes.c, items + 2, 1, NULL, &nodes.g);
  ASSERT_NODE(root_node->right->right, &nodes.g, items + 3, 1, NULL, NULL);
  return 0;
}


/**
 * BEFORE:
 *     p(1)
 *      \
 *       c(1)
 *        \
 *         g(1)
 * AFTER:
 *     c(2)
 *    / \
 *   p   g
 */
static int test_minimal_three_node_rotation(
    void
) {
  const size_t items[] = {0, 1, 2};
  typedef struct {
    aatree_internal_node_t p;
    aatree_internal_node_t c;
    aatree_internal_node_t g;
  } nodes_t;
  nodes_t nodes = (nodes_t){
    .p = {.item = items + 0, .level = 1, .left = NULL, .right = &nodes.c},
    .c = {.item = items + 1, .level = 1, .left = NULL, .right = &nodes.g},
    .g = {.item = items + 2, .level = 1, .left = NULL, .right = NULL},
  };
  aatree_internal_node_t * root_node = &nodes.p;
  assert(0 == aatree_internal_split(&root_node));
  ASSERT_NODE(root_node, &nodes.c, items + 1, 2, &nodes.p, &nodes.g);
  ASSERT_NODE(root_node->left, &nodes.p, items + 0, 1, NULL, NULL);
  ASSERT_NODE(root_node->right, &nodes.g, items + 2, 1, NULL, NULL);
  return 0;
}

/**
 * BEFORE:
 *       r(2)
 *      / \
 *     x   p(1)
 *          \
 *           c(1)
 *            \
 *             g(1)
 * AFTER split(p):
 *       r(2)
 *      / \
 *     x   c(2)
 *        / \
 *       p(1)g(1)
 */
static int test_subtree_rotation_updates_parent_pointer(
    void
) {
  const size_t items[] = {0, 1, 2, 3, 4};
  typedef struct {
    aatree_internal_node_t r;
    aatree_internal_node_t x;
    aatree_internal_node_t p;
    aatree_internal_node_t c;
    aatree_internal_node_t g;
  } nodes_t;
  nodes_t nodes = (nodes_t){
    .r = {.item = items + 1, .level = 2, .left = &nodes.x, .right = &nodes.p},
    .x = {.item = items + 0, .level = 1, .left = NULL, .right = NULL},
    .p = {.item = items + 2, .level = 1, .left = NULL, .right = &nodes.c},
    .c = {.item = items + 3, .level = 1, .left = NULL, .right = &nodes.g},
    .g = {.item = items + 4, .level = 1, .left = NULL, .right = NULL},
  };
  aatree_internal_node_t * root_node = &nodes.r;
  assert(0 == aatree_internal_split(&root_node->right));
  ASSERT_NODE(root_node, &nodes.r, items + 1, 2, &nodes.x, &nodes.c);
  ASSERT_NODE(root_node->left, &nodes.x, items + 0, 1, NULL, NULL);
  ASSERT_NODE(root_node->right, &nodes.c, items + 3, 2, &nodes.p, &nodes.g);
  ASSERT_NODE(root_node->right->left, &nodes.p, items + 2, 1, NULL, NULL);
  ASSERT_NODE(root_node->right->right, &nodes.g, items + 4, 1, NULL, NULL);
  return 0;
}

int main(
    void
) {
  test_double_right_horizontal_links_performs_left_rotation();
  test_null_parent_returns_error();
  test_no_right_child_no_change();
  test_no_right_grandchild_no_change();
  test_right_grandchild_lower_level_no_change();
  test_minimal_three_node_rotation();
  test_subtree_rotation_updates_parent_pointer();
  return 0;
}

#endif // AATREE_SPLIT_TEST_H
