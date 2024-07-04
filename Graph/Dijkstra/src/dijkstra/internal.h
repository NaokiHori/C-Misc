#if !defined(DIJKSTRA_INTERNAL_H)
#define DIJKSTRA_INTERNAL_H

#include <stdbool.h>
#include <stddef.h>

// data types

typedef struct edge_t edge_t;
typedef struct node_t node_t;

struct node_t {
  bool visited;
  size_t index;
  double cost;
  size_t heap_index;
  edge_t * edges;
  node_t * prev;
};

struct edge_t {
  double cost;
  node_t * node;
  edge_t * next;
};

// priority queue

extern node_t ** dijkstra_pqueue_init (
    const size_t nnodes,
    node_t * const nodes
);

extern node_t * dijkstra_pqueue_pull (
    node_t ** pqueue
);

extern int dijkstra_pqueue_update (
    const size_t nnodes,
    node_t ** pqueue,
    const size_t my_index
);

// debug util functions

extern int dijkstra_dump_nodes (
    const size_t nnodes,
    const node_t * nodes
);

extern int dijkstra_dump_pqueue (
    const size_t nnodes,
    node_t ** pqueue
);

#endif // DIJKSTRA_INTERNAL_H
