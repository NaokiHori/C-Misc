#include <stdlib.h>
#include "./aatree.h"
#include "./aatree/check.h"
#include "./aatree/delete.h"
#include "./aatree/destroy.h"
#include "./aatree/display.h"
#include "./aatree/insert.h"
#include "./aatree/node.h"
#include "./aatree/search.h"
#include "./aatree/skew.h"
#include "./aatree/split.h"

struct aatree_t {
  int (* compare)(
      const void * const a,
      const void * const b
  );
  aatree_internal_node_t * root;
};

int aatree_init(
    int (* const compare)(
        const void * const a,
        const void * const b
    ),
    aatree_t ** const aatree
) {
  if (NULL != *aatree) {
    return 1;
  }
  *aatree = malloc(sizeof(aatree_t));
  if (NULL == *aatree) {
    return 1;
  }
  (*aatree)->compare = compare;
  (*aatree)->root = NULL;
  return 0;
}

int aatree_finalize(
    aatree_t ** const aatree
) {
  if (NULL == *aatree) {
    return 1;
  }
  if (0 != aatree_internal_destroy((*aatree)->root)) {
    return 1;
  }
  free(*aatree);
  return 0;
}

int aatree_insert(
    aatree_t * const aatree,
    const void * const item
) {
  if (NULL == aatree) {
    return 1;
  }
  return aatree_internal_insert(aatree->compare, &aatree->root, item);
}

int aatree_delete(
    aatree_t * const aatree,
    const void * const item
) {
  if (NULL == aatree) {
    return 1;
  }
  return aatree_internal_delete(aatree->compare, &aatree->root, item);
}

int aatree_search(
    const aatree_t * const aatree,
    const void * const item,
    bool * const is_present
) {
  if (NULL == aatree) {
    return 1;
  }
  return aatree_internal_search(aatree->compare, aatree->root, item, is_present);
}

int aatree_display(
    int (* const print)(
        const void * item
    ),
    const aatree_t * const aatree
) {
  if (NULL == aatree) {
    return 1;
  }
  return aatree_internal_display(print, aatree->root);
}

#if defined(AATREE_TEST)

#include <assert.h>

/**
 * Fisher–Yates shuffle
 */
static int shuffle(
    const size_t nitems,
    size_t * const items
) {
  // empty arrays: do nothing
  if (0 == nitems) {
    return 0;
  }
  if (NULL == items) {
    return 0;
  }
  for (size_t i = nitems - 1; 0 < i; i -= 1){
    const size_t j = rand() % (i + 1);
    const size_t tmp = items[i];
    items[i] = items[j];
    items[j] = tmp;
  }
  return 0;
}

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

/**
 * integration test
 * 1. prepare numbers
 * 2. initialize tree
 * 3. insert all numbers in a random order and check tree's validity
 * 4. delete all numbers in a random order and check tree's validity
 * 5. finalize tree
 */
int main(
    void
) {
  srand(1);
  const size_t nitems = 1024;
  size_t * const items = malloc(nitems * sizeof(size_t));
  size_t * const indices = malloc(nitems * sizeof(size_t));
  if (NULL == items || NULL == indices) {
    return 1;
  }
  for (size_t i = 0; i < nitems; i++) {
    items[i] = i;
    indices[i] = i;
  }
  assert(0 == shuffle(nitems, indices));
  aatree_t * aatree = NULL;
  assert(0 == aatree_init(compare, &aatree));
  for (size_t i = 0; i < nitems; i++) {
    assert(0 == aatree_insert(aatree, items + indices[i]));
    assert(0 == aatree_internal_check(aatree->root));
  }
  assert(0 == shuffle(nitems, indices));
  for (size_t i = 0; i < nitems; i++) {
    assert(0 == aatree_delete(aatree, items + indices[i]));
    assert(0 == aatree_internal_check(aatree->root));
  }
  // deleted all inserted numbers: the tree should be empty
  assert(NULL == aatree->root);
  assert(0 == aatree_finalize(&aatree));
  free(items);
  free(indices);
  return 0;
}

#endif // AATREE_TEST
