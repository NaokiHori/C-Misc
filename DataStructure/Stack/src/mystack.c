#include <stdio.h>
#include <stdlib.h>
#include "mystack.h"

// memory manager

static void * memory_alloc (
    const size_t size
) {
  void * ptr = malloc(size);
  if (NULL == ptr) {
    printf("failed to allocate memory: %zu\n", size);
    exit(EXIT_FAILURE);
  }
  return ptr;
}

static void memory_free (
    void * ptr
) {
  free(ptr);
}

// singly-linked list

typedef struct node_t {
  struct node_t * next;
  size_t item;
} node_t;

struct mystack_t {
  size_t nitems;
  node_t * root;
};

int mystack_init (
    mystack_t ** mystack
) {
  *mystack = memory_alloc(sizeof(mystack_t));
  (*mystack)->nitems = 0;
  (*mystack)->root = NULL;
  return 0;
}

int mystack_push (
    mystack_t * mystack,
    const size_t item
) {
  if (NULL == mystack) {
    return 1;
  }
  node_t * new_node = memory_alloc(sizeof(node_t));
  new_node->next = mystack->root;
  new_node->item = item;
  mystack->root = new_node;
  mystack->nitems += 1;
  return 0;
}

int mystack_peek (
    mystack_t * mystack,
    size_t * item
) {
  if (NULL == mystack) {
    return 1;
  }
  if (0 == mystack->nitems) {
    return 1;
  }
  *item = mystack->root->item;
  return 0;
}

int mystack_count (
    mystack_t * mystack,
    size_t * nitems
) {
  if (NULL == mystack) {
    return 1;
  }
  *nitems = mystack->nitems;
  return 0;
}

int mystack_pop (
    mystack_t * mystack,
    size_t * item
) {
  if (NULL == mystack) {
    return 1;
  }
  if (0 == mystack->nitems) {
    return 1;
  }
  node_t * node = mystack->root;
  *item = node->item;
  mystack->root = node->next;
  memory_free(node);
  mystack->nitems -= 1;
  return 0;
}

int mystack_finalise (
    mystack_t ** mystack
) {
  if (NULL == *mystack) {
    return 1;
  }
  node_t * node = (*mystack)->root;
  while (node) {
    node_t * next = node->next;
    memory_free(node);
    node = next;
  }
  memory_free(*mystack);
  *mystack = NULL;
  return 0;
}

