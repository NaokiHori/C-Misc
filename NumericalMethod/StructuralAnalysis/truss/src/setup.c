#include <math.h>
#include <stdlib.h>
#include "parameter.h"
#include "./setup.h"

static double compute_length(
    const node_t * const node0,
    const node_t * const node1
) {
  // NOTE: should use hypot (overflow-safe option)
  double squared = 0.;
  for (size_t dim = 0; dim < NDIMS; dim++) {
    squared += pow(node1->position[dim] - node0->position[dim], 2.);
  }
  return sqrt(squared);
}

static int configure_element(
    const node_t * const nodes,
    const size_t node_indices[N_ELEMENT_NODES],
    element_t * const elements,
    size_t * const element_index
) {
  element_t * const element = elements + *element_index;
  const node_t * (* const local_nodes)[N_ELEMENT_NODES] = &element->nodes;
  for (size_t n = 0; n < N_ELEMENT_NODES; n++) {
    (*local_nodes)[n] = nodes + node_indices[n];
  }
  element->length = compute_length(
      (*local_nodes)[0],
      (*local_nodes)[1]
  );
  *element_index += 1;
  return 0;
}

static int setup_truss_bridge(
    size_t * const n_nodes,
    node_t ** const nodes,
    size_t * const n_elements,
    element_t ** const elements
) {
#if NDIMS == 2
  const size_t n = 8;
  *n_nodes = 2 * n;
  *nodes = malloc(*n_nodes * sizeof(node_t));
  for (size_t i = 0; i < *n_nodes; i++) {
    const double height = 0.5;
    const double x = i < n + 1 ? -1. + (2. / n) * i : -1. + (2. / n) * (i - n);
    const double y = i < n + 1 ? 0. : height * (1. - (4. / pow(n, 2.) * pow(i - 1.5 * n, 2.)));
    node_t * const node = *nodes + i;
    node->index = i;
    node->position[0] = x;
    node->position[1] = y;
    // edge nodes are fixed in x
    node->is_fixed[0] = 0 == i || n == i;
    // edge nodes are fixed in y
    node->is_fixed[1] = 0 == i || n == i;
  }
  // bottom chord (tie), top chord (arch), verticals, diagonals
  *n_elements =
    + n
    + n
    + n - 1
    + n - 2;
  *elements = malloc(*n_elements * sizeof(element_t));
  size_t counter = 0;
  for (size_t i = 0; i < n; i++) {
    configure_element(*nodes, (size_t [N_ELEMENT_NODES]){i, i + 1}, *elements, &counter);
  }
  for (size_t i = n + 1; i < 2 * n - 1; i++) {
    configure_element(*nodes, (size_t [N_ELEMENT_NODES]){i, i + 1}, *elements, &counter);
  }
  for (size_t i = 1; i < n; i++) {
    configure_element(*nodes, (size_t [N_ELEMENT_NODES]){i, i + n}, *elements, &counter);
  }
  configure_element(*nodes, (size_t [N_ELEMENT_NODES]){0, n + 1}, *elements, &counter);
  configure_element(*nodes, (size_t [N_ELEMENT_NODES]){n, 2 * n - 1}, *elements, &counter);
  for (size_t i = 1; i < n / 2; i++) {
    configure_element(*nodes, (size_t [N_ELEMENT_NODES]){i + 1, i + n}, *elements, &counter);
    configure_element(*nodes, (size_t [N_ELEMENT_NODES]){n - i - 1, 2 * n - i}, *elements, &counter);
  }
#elif NDIMS == 3
  const size_t n_bays = 4; // Number of longitudinal divisions along X
  const double L = 2.0;    // Total length along X
  const double H = 0.5;    // Box height along Y
  const double W = 0.4;    // Box width along Z
  // 8 nodes per bay section: 4 nodes per frame slice along X
  const size_t nodes_per_slice = 4;
  *n_nodes = (n_bays + 1) * nodes_per_slice; // 20 nodes total
  *nodes = malloc(*n_nodes * sizeof(node_t));
  // 1. GENERATE NODES (4 nodes per cross-section along X)
  size_t node_idx = 0;
  for (size_t i = 0; i <= n_bays; i++) {
    const double x = (L / n_bays) * i;
    // Slice order: Bottom-Front(0), Top-Front(1), Top-Back(2), Bottom-Back(3)
    const double coords[4][2] = {
      {0.0, -W / 2.0}, {H, -W / 2.0}, {H, W / 2.0}, {0.0, W / 2.0}
    };
    for (size_t j = 0; j < 4; j++) {
      node_t * const node = *nodes + node_idx;
      node->index = node_idx;
      node->position[0] = x;
      node->position[1] = coords[j][0];
      node->position[2] = coords[j][1];
      // Simply supported: Pin left end (x=0), Roller right end (x=L)
      node->is_fixed[0] = (i == 0);                // Fix X at x=0
      node->is_fixed[1] = (i == 0 || i == n_bays); // Fix Y at bottom corners
      node->is_fixed[2] = (i == 0 && j == 0);      // Fix Z at single node to prevent rigid slide
      node_idx++;
    }
  }
  // 2. GENERATE ELEMENTS
  // Per bay: 4 longitudinal + 4 transverse + 6 face diagonals = 14 elements/bay + 4 end transverse
  *n_elements = n_bays * 14 + 4;
  *elements = malloc(*n_elements * sizeof(element_t));
  size_t counter = 0;
  // A. End-cap transverse frame at x=0
  for (size_t j = 0; j < 4; j++) {
    size_t n1 = j;
    size_t n2 = (j + 1) % 4;
    configure_element(*nodes, (size_t[N_ELEMENT_NODES]){n1, n2}, *elements, &counter);
  }
  // B. Longitudinal, Transverse, and Diagonal elements per bay
  for (size_t i = 0; i < n_bays; i++) {
    const size_t curr = i * 4;       // Start node index of current slice
    const size_t next = (i + 1) * 4; // Start node index of next slice
    // 1. Longitudinal edges (4 total)
    for (size_t j = 0; j < 4; j++) {
        configure_element(*nodes, (size_t[N_ELEMENT_NODES]){curr + j, next + j}, *elements, &counter);
    }
    // 2. Transverse ring on next slice (4 total)
    for (size_t j = 0; j < 4; j++) {
        size_t n1 = next + j;
        size_t n2 = next + ((j + 1) % 4);
        configure_element(*nodes, (size_t[N_ELEMENT_NODES]){n1, n2}, *elements, &counter);
    }
    // 3. Side face diagonals (Pratt/Warren pattern on 4 outer faces)
    configure_element(*nodes, (size_t[N_ELEMENT_NODES]){curr + 0, next + 1}, *elements, &counter); // Front face
    configure_element(*nodes, (size_t[N_ELEMENT_NODES]){curr + 1, next + 2}, *elements, &counter); // Top face
    configure_element(*nodes, (size_t[N_ELEMENT_NODES]){curr + 2, next + 3}, *elements, &counter); // Back face
    configure_element(*nodes, (size_t[N_ELEMENT_NODES]){curr + 0, next + 3}, *elements, &counter); // Bottom face
    // 4. Internal cross-bracing (prevents cross-sectional racking)
    configure_element(*nodes, (size_t[N_ELEMENT_NODES]){curr + 0, next + 2}, *elements, &counter);
    configure_element(*nodes, (size_t[N_ELEMENT_NODES]){curr + 1, next + 3}, *elements, &counter);
  }
#else
#error "unexpected NDIMS"
#endif
  return 0;
}

int setup(
    size_t * const n_nodes,
    node_t ** const nodes,
    size_t * const n_elements,
    element_t ** const elements,
    double ** const external_forces
) {
  setup_truss_bridge(n_nodes, nodes, n_elements, elements);
  // gravity uniformly pulls down all elements
  *external_forces = malloc((*n_nodes * NDIMS) * sizeof(double));
  for (size_t n = 0; n < *n_nodes; n++) {
    for (size_t dim = 0; dim < NDIMS; dim++) {
      (*external_forces)[n * NDIMS + dim] = 0.;
    }
  }
  for (size_t n = 0; n < *n_elements; n++) {
    element_t * const element = *elements + n;
    const double length = element->length;
    const node_t * (* const local_nodes)[N_ELEMENT_NODES] = &element->nodes;
    for (size_t m = 0; m < N_ELEMENT_NODES; m++) {
      const size_t index = (*local_nodes)[m]->index;
      (*external_forces)[index * NDIMS + 1] += 1. / N_ELEMENT_NODES * ELEMENT_DENSITY * ELEMENT_AREA * length * GRAVITY;
    }
  }
  return 0;
}

