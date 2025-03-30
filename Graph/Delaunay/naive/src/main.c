#include <errno.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include "delaunay.h"
#include "./snpyio.h"

static int output(
    const size_t nnodes,
    const node_t * const nodes,
    const size_t nedges,
    const edge_t * const edges
) {
  // nodes
  {
    const char file_name[] = {"nodes.npy"};
    double * const buf = malloc(2 * nnodes * sizeof(double));
    for (size_t i = 0; i < nnodes; i++) {
      const node_t * const node = nodes + i;
      buf[2 * i + 0] = node->x;
      buf[2 * i + 1] = node->y;
    }
    errno = 0;
    FILE * const fp = fopen(file_name, "w");
    if (NULL == fp) {
      perror(file_name);
      return 1;
    }
    size_t header_size = 0;
    if (0 != snpyio_w_header(2, (size_t []){nnodes, 2}, "'<f8'", false, fp, &header_size)) {
      return 1;
    }
    fwrite(buf, sizeof(double), 2 * nnodes, fp);
    fclose(fp);
    free(buf);
  }
  // edges
  {
    const char file_name[] = {"edges.npy"};
    size_t * const buf = malloc(2 * nedges * sizeof(size_t));
    size_t i = 0;
    for (const edge_t * edge = edges; edge; edge = edge->next) {
      buf[2 * i + 0] = edge->i0;
      buf[2 * i + 1] = edge->i1;
      i += 1;
    }
    errno = 0;
    FILE * const fp = fopen(file_name, "w");
    if (NULL == fp) {
      perror(file_name);
      return 1;
    }
    size_t header_size = 0;
    if (0 != snpyio_w_header(2, (size_t []){nedges, 2}, "'<u8'", false, fp, &header_size)) {
      return 1;
    }
    fwrite(buf, sizeof(size_t), 2 * nedges, fp);
    fclose(fp);
    free(buf);
  }
  return 0;
}

int main(
    void
) {
  srand(1 << 10);
  const size_t nnodes = 16;
  // initialize position of nodes
  node_t * const nodes = malloc(nnodes * sizeof(node_t));
  for (size_t i = 0; i < nnodes; i++) {
    node_t * const node = nodes + i;
    double * const x = &node->x;
    double * const y = &node->y;
    *x = - 0.5 + 1. * rand() / RAND_MAX;
    *y = - 0.5 + 1. * rand() / RAND_MAX;
  }
  edge_t * first_edge = NULL;
  if (0 != delaunay_triangulate(nnodes, nodes, &first_edge)) {
    puts("failed to triangulate");
    return 1;
  }
  size_t nedges = 0;
  for (const edge_t * edge = first_edge; edge; edge = edge->next) {
    nedges += 1;
  }
  printf("%zu nodes, %zu edges\n", nnodes, nedges);
  output(nnodes, nodes, nedges, first_edge);
  free(nodes);
  while (first_edge) {
    edge_t * const next_edge = first_edge->next;
    free(first_edge);
    first_edge = next_edge;
  }
  return 0;
}

