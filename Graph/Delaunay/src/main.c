#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <limits.h>
#include "combinations.h"
#include "snpyio.h"

typedef struct {
  double x;
  double y;
} node_t;

typedef struct {
  size_t i0;
  size_t i1;
} edge_t;

static double pow2 (
    const double v
) {
  return v * v;
}

static int process (
    const size_t nnodes,
    const node_t * nodes,
    size_t * nedges,
    edge_t * edges,
    const size_t * node_indices
) {
  const size_t i0 = node_indices[0];
  const size_t i1 = node_indices[1];
  const size_t i2 = node_indices[2];
  const node_t * n0 = nodes + i0;
  const node_t * n1 = nodes + i1;
  const node_t * n2 = nodes + i2;
  bool is_valid = true;
  for (size_t i3 = 0; i3 < nnodes; i3++) {
    if (i0 == i3 || i1 == i3 || i2 == i3) {
      continue;
    }
    const node_t * n3 = nodes + i3;
    const double cross =
      + (n1->x - n0->x) * (n2->y - n0->y)
      - (n2->x - n0->x) * (n1->y - n0->y);
    const bool rev = 0. < cross;
    const double a00 = n0->x - n3->x;
    const double a01 = n0->y - n3->y;
    const double a02 = pow2(a00) + pow2(a01);
    const double a10 = (rev ? n1->x : n2->x) - n3->x;
    const double a11 = (rev ? n1->y : n2->y) - n3->y;
    const double a12 = pow2(a10) + pow2(a11);
    const double a20 = (rev ? n2->x : n1->x) - n3->x;
    const double a21 = (rev ? n2->y : n1->y) - n3->y;
    const double a22 = pow2(a20) + pow2(a21);
    const double det = a00 * a11 * a22
                     + a10 * a21 * a02
                     + a20 * a01 * a12
                     - a00 * a21 * a12
                     - a10 * a01 * a22
                     - a20 * a11 * a02;
    if (0. < det) {
      is_valid = false;
      break;
    }
  }
  if (is_valid) {
    const size_t node_pairs[3][2] = {
      {i0, i1},
      {i1, i2},
      {i0, i2},
    };
    for (size_t i = 0; i < sizeof(node_pairs) / sizeof(node_pairs[0]); i++) {
      const size_t * node_pair = node_pairs[i];
      // NOTE: O(Nv)
      is_valid = true;
      for (size_t j = 0; j < *nedges; j++) {
        edge_t * edge = edges + j;
        if (node_pair[0] == edge->i0 && node_pair[1] == edge->i1) {
          is_valid = false;
          break;
        }
      }
      if (is_valid) {
        edge_t * edge = edges + *nedges;
        edge->i0 = node_pair[0];
        edge->i1 = node_pair[1];
        *nedges += 1;
      }
    }
  }
  return 0;
}

static int output (
    const size_t nnodes,
    const node_t * nodes,
    const size_t nedges,
    const edge_t * edges
) {
  // nodes
  {
    double * buf = malloc(2 * nnodes * sizeof(double));
    for (size_t i = 0; i < nnodes; i++) {
      const node_t * const node = nodes + i;
      buf[2 * i + 0] = node->x;
      buf[2 * i + 1] = node->y;
    }
    FILE * fp = fopen("nodes.npy", "w");
    snpyio_w_header(2, (size_t []){nnodes, 2}, "'<f8'", false, fp);
    fwrite(buf, sizeof(double), 2 * nnodes, fp);
    fclose(fp);
    free(buf);
  }
  // edges
  {
    size_t * buf = malloc(2 * nedges * sizeof(size_t));
    for (size_t i = 0; i < nedges; i++) {
      const edge_t * const edge = edges + i;
      buf[2 * i + 0] = edge->i0;
      buf[2 * i + 1] = edge->i1;
    }
    FILE * fp = fopen("edges.npy", "w");
    snpyio_w_header(2, (size_t []){nedges, 2}, "'<u8'", false, fp);
    fwrite(buf, sizeof(size_t), 2 * nedges, fp);
    fclose(fp);
    free(buf);
  }
  return 0;
}

int main (
    void
) {
  srand(0);
  const size_t N_TRI_VERTICES = 3;
  const size_t nnodes = 128;
  node_t * nodes = malloc(nnodes * sizeof(node_t));
  for (size_t i = 0; i < nnodes; i++) {
    node_t * const node = nodes + i;
generate:
    node->x = - 0.5 + 1. * rand() / RAND_MAX;
    node->y = - 0.5 + 1. * rand() / RAND_MAX;
    if (pow2(0.5) < pow2(node->x) + pow2(node->y)) {
      goto generate;
    }
  }
  size_t * node_indices = malloc(N_TRI_VERTICES * sizeof(size_t));
  size_t nedges = 0;
  edge_t * edges = malloc((nnodes * (nnodes + 1) / 2) * sizeof(edge_t));
  for (size_t i = 0; i < nnodes * (nnodes + 1) / 2; i++) {
    edge_t * const edge = edges + i;
    edge->i0 = SIZE_MAX;
    edge->i1 = SIZE_MAX;
  }
  // get initial combination
  init_combination(N_TRI_VERTICES, node_indices);
  process(nnodes, nodes, &nedges, edges, node_indices);
  // find next combination until reached the last pair
  while (1) {
    if (0 != find_next_combination(nnodes, N_TRI_VERTICES, node_indices)) {
      break;
    }
    process(nnodes, nodes, &nedges, edges, node_indices);
  }
  printf("%zu nodes, %zu edges\n", nnodes, nedges);
  output(nnodes, nodes, nedges, edges);
  free(node_indices);
  free(nodes);
  return 0;
}

