#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "mybst.h"

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

// binary tree node

typedef struct node_t {
  size_t item;
  struct node_t * lnode;
  struct node_t * rnode;
} node_t;

struct mybst_t {
  size_t nitems;
  node_t * root;
};

// operations

int mybst_init (
    mybst_t ** mybst
) {
  *mybst = memory_alloc(sizeof(mybst_t));
  (*mybst)->nitems = 0;
  (*mybst)->root = NULL;
  return 0;
}

int mybst_insert (
    mybst_t * mybst,
    const size_t item
) {
  if (NULL == mybst) {
    return 1;
  }
  node_t ** pnode = &mybst->root;
  while (*pnode) {
    const size_t pitem = (*pnode)->item;
    if (item == pitem) {
      puts("identical items are not allowed for now");
      return 1;
    }
    if (item < pitem) {
      pnode = &(*pnode)->lnode;
    } else {
      pnode = &(*pnode)->rnode;
    }
  }
  *pnode = memory_alloc(sizeof(node_t));
  (*pnode)->item = item;
  (*pnode)->lnode = NULL;
  (*pnode)->rnode = NULL;
  mybst->nitems += 1;
  return 0;
}

int mybst_search (
    const mybst_t * mybst,
    const size_t item
) {
  if (NULL == mybst) {
    return 1;
  }
  node_t * node = mybst->root;
  while (node) {
    if (item == node->item) {
      return 0;
    } else if (item < node->item && node->lnode) {
      node = node->lnode;
    } else {
      node = node->rnode;
    }
  }
  return 1;
}

int mybst_delete (
    mybst_t * mybst,
    const size_t item
) {
  if (NULL == mybst) {
    return 1;
  }
  node_t ** pnode = &mybst->root;
  while (*pnode) {
    const size_t pitem = (*pnode)->item;
    if (item < pitem) {
      pnode = &(*pnode)->lnode;
    } else if (pitem < item) {
      pnode = &(*pnode)->rnode;
    } else {
      // identical item: this node is subject to deletion
      node_t * node = *pnode;
      node_t ** lnode = &node->lnode;
      node_t ** rnode = &node->rnode;
      if (NULL == *lnode && NULL == *rnode) {
        *pnode = NULL;
      } else if (NULL == *lnode) {
        *pnode = *rnode;
      } else if (NULL == *rnode) {
        *pnode = *lnode;
      } else {
        // replace with successor
        // NOTE: update rnode such that it stores a pointer to the successor
        while ((*rnode)->lnode) {
          rnode = &(*rnode)->lnode;
        }
        // for simplicity, swap items only and delete successor
        node->item = (*rnode)->item;
        node = *rnode;
        *rnode = node->rnode;
      }
      mybst->nitems -= 1;
      memory_free(node);
      return 0;
    }
  }
  // not found
  return 1;
}

int mybst_finalise (
    mybst_t ** mybst
) {
  if (NULL == *mybst) {
    return 1;
  }
  while ((*mybst)->root) {
    mybst_delete(*mybst, (*mybst)->root->item);
  }
  memory_free(*mybst);
  *mybst = NULL;
  return 0;
}

