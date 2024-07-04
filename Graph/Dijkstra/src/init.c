#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>
#include "dijkstra.h"
#include "init.h"

int init_graph (
    const size_t nnodes,
    size_t * nlinks,
    link_t ** links
) {
  const size_t nx = 32;
  const size_t ny = 32;
  size_t cntr = nx * ny;
  bool * are_enabled = malloc(nx * ny * sizeof(bool));
  size_t * node_indices = malloc(nx * ny * sizeof(size_t));
  for (size_t n = 0; n < cntr; n++) {
    are_enabled[n] = true;
  }
  for (size_t n = 0; n < cntr; n++) {
    node_indices[n] = SIZE_MAX;
  }
  while (nnodes < cntr) {
    const size_t i = rand() % nx;
    const size_t j = rand() % ny;
    bool * const is_enabled = are_enabled + j * nx + i;
    if (*is_enabled) {
      cntr -= 1;
    }
    *is_enabled = false;
  }
  for (size_t cnt = 0, j = 0; j < ny; j++) {
    for (size_t i = 0; i < nx; i++) {
      if (are_enabled[j * nx + i]) {
        node_indices[j * nx + i] = cnt;
        cnt += 1;
      }
    }
  }
  *nlinks = 0;
  for (size_t j = 0; j < ny - 1; j++) {
    for (size_t i = 0; i < nx - 1; i++) {
      if (!are_enabled[j * nx + i]) {
        continue;
      }
      if (are_enabled[j * nx + (i + 1)]) {
        *nlinks += 1;
      }
      if (are_enabled[(j + 1) * nx + i]) {
        *nlinks += 1;
      }
    }
  }
  *links = malloc(*nlinks * sizeof(link_t));
  cntr = 0;
  for (size_t j = 0; j < ny - 1; j++) {
    for (size_t i = 0; i < nx - 1; i++) {
      if (!are_enabled[j * nx + i]) {
        continue;
      }
      if (are_enabled[j * nx + (i + 1)]) {
        link_t * link = *links + cntr;
        link->node0 = node_indices[j * nx + i];
        link->node1 = node_indices[j * nx + i + 1];
        link->cost = 1.;
        cntr += 1;
      }
      if (are_enabled[(j + 1) * nx + i]) {
        link_t * link = *links + cntr;
        link->node0 = node_indices[j * nx + i];
        link->node1 = node_indices[(j + 1) * nx + i];
        link->cost = 1.;
        cntr += 1;
      }
    }
  }
  {
    FILE * const fp = fopen("graph.txt", "w");
    fprintf(fp, "graph {\n");
    for (size_t j = 0; j < ny; j++) {
      for (size_t i = 0; i < nx; i++) {
        if (are_enabled[j * nx + i]) {
          fprintf(fp, "  %zu [shape=circle pos=\"%zu,%zu!\"];\n", node_indices[j * nx + i], i, j);
        }
      }
    }
    fprintf(fp, "\n");
    for (size_t n = 0; n < *nlinks; n++) {
      const link_t * const link = *links + n;
      fprintf(fp, "%4zu -- %4zu;\n", link->node0, link->node1);
    }
    fprintf(fp, "}\n");
    fclose(fp);
  }
  free(are_enabled);
  free(node_indices);
  return 0;
}

