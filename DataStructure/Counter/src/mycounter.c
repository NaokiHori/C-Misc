#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <string.h>
#include "mycounter.h"

// memory manager

static void * memory_alloc (
    const size_t size
) {
  void * ptr = malloc(size);
  if (ptr == NULL) {
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
  size_t cntr;
} node_t;

typedef struct {
  node_t * root;
} list_t;

static int list_add (
    list_t * list,
    const size_t item
) {
  // check if this item already exists in the list
  node_t * node = list->root;
  while (node) {
    if (node->item == item) {
      node->cntr += 1;
      return 0;
    }
    node = node->next;
  }
  // a new item, prepare a new node and update root
  node_t * new_node = memory_alloc(sizeof(node_t));
  new_node->next = list->root;
  new_node->item = item;
  new_node->cntr = 1;
  list->root = new_node;
  return 0;
}

static int list_get (
    const list_t * list,
    const size_t item,
    size_t * cntr
) {
  node_t * node = list->root;
  while (node) {
    if (node->item == item) {
      *cntr = node->cntr;
      return 0;
    }
    node = node->next;
  }
  // not found: counter is 0
  *cntr = 0;
  return 0;
}

// hash table

struct mycounter_t {
  size_t table_size;
  list_t * buckets;
};

int mycounter_init (
    mycounter_t ** mycounter
) {
  // default table size
  const size_t table_size = 10007;
  *mycounter = memory_alloc(sizeof(mycounter_t));
  (*mycounter)->table_size = table_size;
  list_t ** buckets = &(*mycounter)->buckets;
  *buckets = memory_alloc(table_size * sizeof(list_t));
  // initialise linked list
  for (size_t n = 0; n < table_size; n++) {
    list_t * bucket = (*buckets) + n;
    bucket->root = NULL;
  }
  return 0;
}

static size_t apply_hash_function (
    const mycounter_t * mycounter,
    const size_t item
) {
  const size_t table_size = mycounter->table_size;
  return item % table_size;
}

int mycounter_add (
    mycounter_t * mycounter,
    const size_t item
) {
  if (NULL == mycounter) {
    return 1;
  }
  const size_t index = apply_hash_function(mycounter, item);
  list_t * bucket = mycounter->buckets + index;
  list_add(bucket, item);
  return 0;
}

int mycounter_get (
    const mycounter_t * mycounter,
    const size_t item,
    size_t * cntr
) {
  if (NULL == mycounter) {
    return 1;
  }
  const size_t index = apply_hash_function(mycounter, item);
  const list_t * bucket = mycounter->buckets + index;
  return list_get(bucket, item, cntr);
}

int mycounter_finalise (
    mycounter_t ** mycounter
) {
  if (NULL == *mycounter) {
    return 1;
  }
  const size_t table_size = (*mycounter)->table_size;
  for (size_t index = 0; index < table_size; index++) {
    list_t * bucket = (*mycounter)->buckets + index;
    node_t * node = bucket->root;
    while (node) {
      node_t * next = node->next;
      memory_free(node);
      node = next;
    }
  }
  memory_free((*mycounter)->buckets);
  memory_free(*mycounter);
  *mycounter = NULL;
  return 0;
}

