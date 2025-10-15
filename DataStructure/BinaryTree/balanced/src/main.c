#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>
#include "aatree.h"

static int compare(
    const void * const void_ptr_a,
    const void * const void_ptr_b
) {
  const char * const a = (char *)void_ptr_a;
  const char * const b = (char *)void_ptr_b;
  const int strcmped = strcmp(a, b);
  if (strcmped < 0) {
    return -1;
  } else if (0 < strcmped) {
    return 1;
  } else {
    return 0;
  }
}

static int print(
    const void * const item
) {
  printf("%s", (char *)item);
  return 0;
}

int main(
    void
) {
  const char * items[] = {
    "lorem",
    "ipsum",
    "dolor",
    "sit",
    "amet",
    "consectetur",
    "adipiscing",
    "elit",
    "sed",
    "do",
    "eiusmod",
    "tempor",
    "incididunt",
    "ut",
    "labore",
    "et",
    "dolore",
    "magna",
    "aliqua",
  };
  // initialize a tree
  aatree_t * aatree = NULL;
  if (0 != aatree_init(compare, &aatree)) {
    puts("failed to initialize an AA tree");
    return 1;
  }
  // insert items sequentially to the tree
  for (size_t n = 0; n < sizeof(items) / sizeof(items[0]); n++) {
    const char * const item = items[n];
    if (0 != aatree_insert(aatree, item)) {
      printf("failed to insert %s to the tree\n", item);
      return 1;
    }
  }
  // look for an item in the tree
  bool is_present = false;
  const char * item = "tempor";
  if (0 != aatree_search(aatree, item, &is_present)) {
    printf("failed to search %s in the tree\n", item);
    return 1;
  }
  printf("item %s is %s in the tree\n", item, is_present ? "present" : "absent");
  // visualize the tree
  if (0 != aatree_display(print, aatree)) {
    puts("failed to display the tree");
    return 1;
  }
  // delete items sequentially from the tree
  for (size_t n = 0; n < sizeof(items) / sizeof(items[0]); n++) {
    const char * const item = items[n];
    if (0 != aatree_delete(aatree, item)) {
      printf("failed to delete %s from the tree\n", item);
      return 1;
    }
  }
  // clean-up the tree
  if (0 != aatree_finalize(&aatree)) {
    puts("failed to finalize the tree");
    return 1;
  }
  return 0;
}

