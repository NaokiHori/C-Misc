#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include "delaunay.h"
#include "./combination.h"

// number of vertices of a triangle: 3
#define N_TRI_VERTICES 3

static double pow2(
    const double v
) {
  return v * v;
}

static int append_new_edge_to_list(
    const node_t * const nodes[2],
    edge_t ** const edges
) {
  // do not append this edge
  //   if the combination of the nodes is already registered
  for (const edge_t * edge = *edges; edge; edge = edge->next) {
    if (nodes[0] == edge->nodes[0] && nodes[1] == edge->nodes[1]) {
      return 0;
    }
    if (nodes[0] == edge->nodes[1] && nodes[1] == edge->nodes[0]) {
      return 0;
    }
  }
  edge_t * const new_edge = malloc(sizeof(edge_t));
  new_edge->nodes[0] = nodes[0];
  new_edge->nodes[1] = nodes[1];
  new_edge->next = *edges;
  *edges = new_edge;
  return 0;
}

static int process(
    const size_t nnodes,
    const node_t * const nodes,
    const node_t * const selected_nodes[N_TRI_VERTICES],
    edge_t ** const edges
) {
  // check the set of three nodes serves as a valid triangle element
  const node_t * const n0 = selected_nodes[0];
  const node_t * const n1 = selected_nodes[1];
  const node_t * const n2 = selected_nodes[2];
  const double cross =
    + (n1->x - n0->x) * (n2->y - n0->y)
    - (n2->x - n0->x) * (n1->y - n0->y);
  const bool reverse = 0. < cross;
  // we consider a triangle to be valid if no other points are inside
  for (size_t i3 = 0; i3 < nnodes; i3++) {
    const node_t * const n3 = nodes + i3;
    if (n0 == n3 || n1 == n3 || n2 == n3) {
      continue;
    }
    // ref: https://en.wikipedia.org/wiki/Delaunay_triangulation#Algorithms
    const double a00 = n0->x - n3->x;
    const double a01 = n0->y - n3->y;
    const double a02 = pow2(a00) + pow2(a01);
    const double a10 = (reverse ? n1->x : n2->x) - n3->x;
    const double a11 = (reverse ? n1->y : n2->y) - n3->y;
    const double a12 = pow2(a10) + pow2(a11);
    const double a20 = (reverse ? n2->x : n1->x) - n3->x;
    const double a21 = (reverse ? n2->y : n1->y) - n3->y;
    const double a22 = pow2(a20) + pow2(a21);
    const double det = a00 * a11 * a22
                     + a10 * a21 * a02
                     + a20 * a01 * a12
                     - a00 * a21 * a12
                     - a10 * a01 * a22
                     - a20 * a11 * a02;
    if (0. < det) {
      // another point is inside this triangle
      return 0;
    }
  }
  // add three edges composed of the three nodes to list of Delaunay edges
  append_new_edge_to_list((const node_t * [2]){n0, n1}, edges);
  append_new_edge_to_list((const node_t * [2]){n1, n2}, edges);
  append_new_edge_to_list((const node_t * [2]){n0, n2}, edges);
  return 0;
}

int delaunay_triangulate(
    const size_t nnodes,
    const node_t * const nodes,
    edge_t ** const first_edge
) {
  // three nodes are chosen from the whole nodes
  // namely check the entire "n C r" combinations
  size_t selected_node_indices[N_TRI_VERTICES] = {SIZE_MAX, SIZE_MAX, SIZE_MAX};
  init_combination(N_TRI_VERTICES, selected_node_indices);
  while (1) {
    process(
        nnodes,
        nodes,
        (const node_t * [N_TRI_VERTICES]){
          nodes + selected_node_indices[0],
          nodes + selected_node_indices[1],
          nodes + selected_node_indices[2],
        },
        first_edge
    );
    // move on to the next combination, until reached the last pair
    if (0 != find_next_combination(nnodes, N_TRI_VERTICES, selected_node_indices)) {
      break;
    }
  }
  return 0;
}

