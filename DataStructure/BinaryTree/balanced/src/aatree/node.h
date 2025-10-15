#if !defined(AATREE_NODE_H)
#define AATREE_NODE_H

#include <stddef.h>

typedef struct aatree_internal_node_t {
  const void * item;
  size_t level;
  struct aatree_internal_node_t * left;
  struct aatree_internal_node_t * right;
} aatree_internal_node_t;

#endif // AATREE_NODE_H
