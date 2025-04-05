#include <errno.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include "delaunay.h"

static int output(
    const size_t nnodes,
    const node_t * const nodes,
    const edge_t * const edges
) {
  // nodes
  {
    const char file_name[] = {"nodes.dat"};
    errno = 0;
    FILE * const fp = fopen(file_name, "w");
    if (NULL == fp) {
      perror(file_name);
      return 1;
    }
    for (size_t i = 0; i < nnodes; i++) {
      const node_t * const node = nodes + i;
      fprintf(fp, "% .15e % .15e\n", node->x, node->y);
    }
    fclose(fp);
  }
  // edges
  {
    const char file_name[] = {"edges.dat"};
    errno = 0;
    FILE * const fp = fopen(file_name, "w");
    if (NULL == fp) {
      perror(file_name);
      return 1;
    }
    for (const edge_t * edge = edges; edge; edge = edge->next) {
      fprintf(
          fp,
          "%zu %zu % .15e % .15e % .15e % .15e\n",
          edge->nodes[0]->index,
          edge->nodes[1]->index,
          edge->nodes[0]->x,
          edge->nodes[0]->y,
          edge->nodes[1]->x,
          edge->nodes[1]->y
      );
    }
    fclose(fp);
  }
  return 0;
}

int main(
    void
) {
  srand(0);
  const size_t nnodes = 16;
  // initialize position of nodes
  node_t * const nodes = malloc(nnodes * sizeof(node_t));
  for (size_t i = 0; i < nnodes; i++) {
    node_t * const node = nodes + i;
    size_t * const index = &node->index;
    double * const x = &node->x;
    double * const y = &node->y;
    *index = i;
    *x = - 0.5 + 1. * rand() / RAND_MAX;
    *y = - 0.5 + 1. * rand() / RAND_MAX;
  }
  edge_t * first_edge = NULL;
  if (0 != delaunay_triangulate(nnodes, nodes, &first_edge)) {
    puts("failed to triangulate");
    return 1;
  }
  output(nnodes, nodes, first_edge);
  free(nodes);
  while (first_edge) {
    edge_t * const next_edge = first_edge->next;
    free(first_edge);
    first_edge = next_edge;
  }
  return 0;
}

