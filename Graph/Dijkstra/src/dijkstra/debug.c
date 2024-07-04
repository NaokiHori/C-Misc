#include <stdio.h>
#include <stdbool.h>
#include "internal.h"

static const bool debug = true;

int dijkstra_dump_nodes (
    const size_t nnodes,
    const node_t * nodes
) {
  if (!debug) {
    return 0;
  }
  printf("%s\n", __func__);
  for (size_t i = 0; i < nnodes; i++) {
    const node_t * node = nodes + i;
    if (node->index != i) {
      printf("ERROR: node index %zu and array index %zu do not match\n", node->index, i);
    }
    printf("node %3zu: (visited: %5s, heap index %3zu, cost % .15e)\n", node->index, node->visited ? "true" : "false", node->heap_index, node->cost);
  }
  return 0;
}

int dijkstra_dump_pqueue (
    const size_t nnodes,
    node_t ** pqueue
) {
  if (!debug) {
    return 0;
  }
  printf("%s\n", __func__);
  for (size_t i = 0; i < nnodes; i++) {
    const node_t * node = pqueue[i];
    if (node->heap_index != i) {
      printf("ERROR: heap index %zu and array index %zu do not match\n", node->heap_index, i);
    }
    printf("queue %3zu: (node %3zu, cost % .15e)\n", i, node->index, node->cost);
  }
  return 0;
}

