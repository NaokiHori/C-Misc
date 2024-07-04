#include <stdio.h>
#include <stdbool.h>
#include "memory.h"
#include "dijkstra.h"
#include "internal.h"

static int init_edge (
    edge_t ** const root_edge,
    const double cost,
    node_t * node
) {
  // singly-linked list
  edge_t * const new_edge = memory_alloc(1, sizeof(edge_t));
  new_edge->cost = cost;
  new_edge->node = node;
  new_edge->next = *root_edge;
  *root_edge = new_edge;
  return 0;
}

static int init (
    const size_t nnodes,
    const size_t nlinks,
    const link_t * const links,
    node_t ** const nodes
) {
  *nodes = memory_alloc(nnodes, sizeof(node_t));
  for (size_t i = 0; i < nnodes; i++) {
    node_t * const node = *nodes + i;
    node->index = i;
    node->edges = NULL;
    node->heap_index = i;
    node->prev = NULL;
  }
  for (size_t i = 0; i < nlinks; i++) {
    const link_t * const link = links + i;
    const size_t node0 = link->node0;
    const size_t node1 = link->node1;
    const double cost = link->cost;
    init_edge(&(*nodes)[node0].edges, cost, *nodes + node1);
    init_edge(&(*nodes)[node1].edges, cost, *nodes + node0);
  }
  return 0;
}

// ref: https://en.wikipedia.org/wiki/Dijkstra%27s_algorithm#Algorithm
static int process (
    const size_t start,
    const size_t nnodes,
    node_t * nodes
) {
  // mark all nodes as unvisited and
  //   assign a cost from the start node to all nodes
  for (size_t i = 0; i < nnodes; i++) {
    node_t * node = nodes + i;
    node->visited = false;
    node->cost = start == i ? 0. : DIJKSTRA_INFTY_COST;
  }
  // prepare a priority queue to fetch a node to examine in O(1)
  node_t ** pqueue = dijkstra_pqueue_init(nnodes, nodes);
  while (1) {
    // select a node which is unvisited and having the smallest finite cost
    node_t * node = dijkstra_pqueue_pull(pqueue);
    if (NULL == node) {
      // we have visited all nodes
      break;
    }
    // for each neighbour, update cost from the start node
    for (const edge_t * edge = node->edges; edge; edge = edge->next) {
      node_t * neighbour_node = edge->node;
      // compare two routes and update if the new one is shorter
      double * cost = &neighbour_node->cost;
      const double cost_candidate = node->cost + edge->cost;
      if (cost_candidate < *cost) {
        *cost = cost_candidate;
        neighbour_node->prev = node;
        // update priority queue as well
        dijkstra_pqueue_update(nnodes, pqueue, neighbour_node->heap_index);
      }
    }
    node->visited = true;
    dijkstra_pqueue_update(nnodes, pqueue, node->heap_index);
  }
  printf("minimum cost from node %zu to each node and path\n", start);
  for (size_t i = 0; i < nnodes; i++) {
    const node_t * node = nodes + i;
    printf("\tnode %3zu: cost % .15e, path: ", node->index, node->cost);
    while (1) {
      node = node->prev;
      if (!node) {
        break;
      }
      printf("%zu ", node->index);
    }
    puts("");
  }
  memory_free(pqueue);
  return 0;
}

static int finalise (
    const size_t nnodes,
    node_t * nodes
) {
  for (size_t i = 0; i < nnodes; i++) {
    node_t * node = nodes + i;
    for (edge_t * edge = node->edges; edge;) {
      edge_t * next = edge->next;
      memory_free(edge);
      edge = next;
    }
  }
  memory_free(nodes);
  return 0;
}

// 1. construct graph from input
// 2. find shortest path for each node
int dijkstra (
    const size_t nnodes,
    const size_t nlinks,
    const link_t * const links
) {
  const size_t start = 0;
  node_t * nodes = NULL;
  if (0 != init(nnodes, nlinks, links, &nodes)) {
    goto abort;
  }
  if (0 != process(start, nnodes, nodes)) {
    goto abort;
  }
abort:
  if (0 != finalise(nnodes, nodes)) {
    return 1;
  }
  return 0;
}

