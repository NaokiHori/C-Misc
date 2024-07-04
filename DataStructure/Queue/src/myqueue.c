#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "myqueue.h"

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

// queue, storing pointers to
//   1. the first item
//   2. the pointer to the "next" member of the last item
struct myqueue_t {
  size_t nitems;
  node_t * head;
  node_t ** tail;
};

int make_empty (
    myqueue_t * myqueue
) {
  // NOTE: tail treatment is to make assignment to *(myqueue->tail) updating link
  myqueue->nitems = 0;
  myqueue->head = NULL;
  myqueue->tail = &myqueue->head;
  return 0;
}

int myqueue_init (
    myqueue_t ** myqueue
) {
  *myqueue = memory_alloc(sizeof(myqueue_t));
  make_empty(*myqueue);
  return 0;
}

int myqueue_enqueue (
    myqueue_t * myqueue,
    const size_t item
) {
  if (NULL == myqueue) {
    return 1;
  }
  node_t * new_node = memory_alloc(sizeof(node_t));
  new_node->item = item;
  new_node->next = NULL;
  *(myqueue->tail) = new_node;
  myqueue->tail = &new_node->next;
  myqueue->nitems += 1;
  return 0;
}

int myqueue_peek (
    myqueue_t * myqueue,
    size_t * item
) {
  if (NULL == myqueue) {
    return 1;
  }
  if (0 == myqueue->nitems) {
    return 1;
  }
  *item = myqueue->head->item;
  return 0;
}

int myqueue_count (
    myqueue_t * myqueue,
    size_t * nitems
) {
  if (NULL == myqueue) {
    return 1;
  }
  *nitems = myqueue->nitems;
  return 0;
}

int myqueue_dequeue (
    myqueue_t * myqueue,
    size_t * item
) {
  if (NULL == myqueue) {
    return 1;
  }
  if (0 == myqueue->nitems) {
    return 1;
  }
  node_t * node = myqueue->head;
  *item = node->item;
  myqueue->head = node->next;
  memory_free(node);
  myqueue->nitems -= 1;
  if (0 == myqueue->nitems) {
    make_empty(myqueue);
  }
  return 0;
}

int myqueue_finalise (
    myqueue_t ** myqueue
) {
  if (NULL == *myqueue) {
    return 1;
  }
  node_t * node = (*myqueue)->head;
  while (node) {
    node_t * next = node->next;
    memory_free(node);
    node = next;
  }
  memory_free(*myqueue);
  *myqueue = NULL;
  return 0;
}

