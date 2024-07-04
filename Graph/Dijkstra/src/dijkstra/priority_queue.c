#include <stdio.h>
#include <stdbool.h>
#include "dijkstra.h"
#include "memory.h"
#include "internal.h"

static const bool debug = false;

static size_t parent (
    const size_t index
) {
  return (index - 1) / 2;
}

static size_t l_child (
    const size_t index
) {
  return 2 * index + 1;
}

static size_t r_child (
    const size_t index
) {
  return 2 * index + 2;
}

static double get_cost (
    const size_t nnodes,
    node_t ** nodes,
    const size_t index
) {
  if (nnodes - 1 < index) {
    return DIJKSTRA_INFTY_COST;
  }
  const node_t * node = nodes[index];
  if (node->visited) {
    return DIJKSTRA_INFTY_COST;
  }
  return node->cost;
}

static int swap (
    node_t ** pqueue,
    const size_t i,
    const size_t j
) {
  node_t * const tmp = *(pqueue + i);
  *(pqueue + i) = *(pqueue + j);
  *(pqueue + j) = tmp;
  // heap index of the node is updated as well
  pqueue[i]->heap_index = i;
  pqueue[j]->heap_index = j;
  return 0;
}

static int upshift (
    const size_t nnodes,
    node_t ** pqueue,
    size_t index_c
) {
  const double cost_c = get_cost(nnodes, pqueue, index_c);
  while (0 < index_c && index_c < nnodes) {
    const size_t index_p = parent(index_c);
    const double cost_p = get_cost(nnodes, pqueue, index_p);
    if (cost_c < cost_p) {
      swap(pqueue, index_c, index_p);
      index_c = index_p;
    } else {
      break;
    }
  }
  return 0;
}

static int downshift (
    const size_t nnodes,
    node_t ** pqueue,
    size_t index_p
) {
  const double cost_p = get_cost(nnodes, pqueue, index_p);
  while (index_p < nnodes) {
    const size_t index_l = l_child(index_p);
    const size_t index_r = r_child(index_p);
    const double cost_l = get_cost(nnodes, pqueue, index_l);
    const double cost_r = get_cost(nnodes, pqueue, index_r);
    if (cost_p <= cost_l && cost_p <= cost_r) {
      break;
    }
    if (cost_l < cost_r) {
      swap(pqueue, index_l, index_p);
      index_p = index_l;
    } else {
      swap(pqueue, index_r, index_p);
      index_p = index_r;
    }
  }
  return 0;
}

static int dijkstra_check_heapified (
    const size_t nnodes,
    node_t ** pqueue
) {
  if (!debug) {
    return 0;
  }
  bool is_heapified = true;
  // check parent -> children relations
  for (size_t index_p = 0; index_p < nnodes; index_p++) {
    const size_t index_l = l_child(index_p);
    const size_t index_r = r_child(index_p);
    const double cost_p = get_cost(nnodes, pqueue, index_p);
    const double cost_l = get_cost(nnodes, pqueue, index_l);
    const double cost_r = get_cost(nnodes, pqueue, index_r);
    if (cost_p <= cost_l && cost_p <= cost_r) {
      continue;
    }
    is_heapified = false;
    printf("invalid at %zu: expect % .15e <= % .15e && % .15e <= % .15e\n",
        index_p,
        cost_p,
        cost_l,
        cost_p,
        cost_r
    );
  }
  // check child -> parent relations
  for (size_t index_c = 1; index_c < nnodes; index_c++) {
    const size_t index_p = parent(index_c);
    const double cost_c = get_cost(nnodes, pqueue, index_c);
    const double cost_p = get_cost(nnodes, pqueue, index_p);
    if (cost_p <= cost_c) {
      continue;
    }
    is_heapified = false;
    printf("invalid at %zu: expect % .15e <= % .15e\n",
        index_c,
        cost_p,
        cost_c
    );
  }
  if (!is_heapified) {
    dijkstra_dump_pqueue(nnodes, pqueue);
  }
  return 0;
}

node_t ** dijkstra_pqueue_init (
    const size_t nnodes,
    node_t * const nodes
) {
  node_t ** pqueue = memory_alloc(nnodes, sizeof(node_t *));
  for (size_t i = 0; i < nnodes; i++) {
    pqueue[i] = nodes + i;
  }
  for (size_t i = 0; i < nnodes; i++) {
    upshift(nnodes, pqueue, i);
  }
  return pqueue;
}

int dijkstra_pqueue_update (
    const size_t nnodes,
    node_t ** pqueue,
    const size_t my_index
) {
  // check if my parent is larger than my (child) cost
  if (0 != my_index) {
    const double cost_c = get_cost(nnodes, pqueue, my_index);
    const double cost_p = get_cost(nnodes, pqueue, parent(my_index));
    if (cost_c < cost_p) {
      upshift(nnodes, pqueue, my_index);
      return 0;
    }
  }
  // check if my child(ren) has smaller cost(s) than my (parent) cost
  {
    const double cost_p = get_cost(nnodes, pqueue, my_index);
    const double cost_l = get_cost(nnodes, pqueue, l_child(my_index));
    const double cost_r = get_cost(nnodes, pqueue, r_child(my_index));
    if (cost_l < cost_p || cost_r < cost_p) {
      downshift(nnodes, pqueue, my_index);
      return 0;
    }
  }
  dijkstra_check_heapified(nnodes, pqueue);
  return 0;
}

node_t * dijkstra_pqueue_pull (
    node_t ** pqueue
) {
  // return heap-top if it meets requirements
  node_t * node = pqueue[0];
  if (node->visited) {
    return NULL;
  }
  return node;
}

