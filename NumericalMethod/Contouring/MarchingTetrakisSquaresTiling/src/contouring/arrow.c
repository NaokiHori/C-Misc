#include <stdio.h> // puts
#include <stddef.h> // size_t
#include <stdbool.h> // bool
#include <stdint.h> // uint_fast8_t
#include "./types.h"
#include "./memory.h"
#include "./arrow.h" // arrow_t
#include "./edge.h" // edge_t
#include "./triangle.h"

static int define_arrow (
    const edge_t * const edges[NUM_TRIANGLE_EDGES],
    triangle_t * const triangles[NUM_TRIANGLE_EDGES],
    const triangle_edge_index_t tail,
    const triangle_edge_index_t head,
    arrow_t ** const arrow
) {
  if (NULL == edges[tail]) {
    puts("unexpected NULL tail edge");
    return 1;
  }
  if (NULL == edges[head]) {
    puts("unexpected NULL head edge");
    return 1;
  }
  *arrow = memory_alloc(1 * sizeof(arrow_t));
  (*arrow)->visited = false;
  (*arrow)->edge_tail = edges[tail];
  (*arrow)->edge_head = edges[head];
  (*arrow)->triangle_tail = triangles[tail];
  (*arrow)->triangle_head = triangles[head];
  return 0;
}

int find_arrow_in_triangle (
    const double threshold,
    const double vertex_values[NUM_TRIANGLE_VERTICES],
    const edge_t * const edges[NUM_TRIANGLE_EDGES],
    triangle_t * const triangles[NUM_TRIANGLE_EDGES],
    arrow_t ** const arrow
) {
  // 1 << NUM_TRIANGLE_VERTICES bits: 2 ^ NUM_TRIANGLE_VERTICES = 8 patterns
  uint_fast8_t vertex_patterns = 0;
  for (size_t n = 0; n < NUM_TRIANGLE_VERTICES; n++) {
    const uint8_t is_above = threshold < vertex_values[n] ? 1 : 0;
    vertex_patterns |= (1 & is_above) << n;
  }
  // being above threshold always come to the right-hand side of arrow
  if (1 == vertex_patterns) {
    return define_arrow(edges, triangles, TRIANGLE_EDGE_45_LEFT,  TRIANGLE_EDGE_45_RIGHT, arrow);
  } else if ( 2 == vertex_patterns) {
    return define_arrow(edges, triangles, TRIANGLE_EDGE_45_RIGHT, TRIANGLE_EDGE_90,       arrow);
  } else if ( 3 == vertex_patterns) {
    return define_arrow(edges, triangles, TRIANGLE_EDGE_45_LEFT,  TRIANGLE_EDGE_90,       arrow);
  } else if ( 4 == vertex_patterns) {
    return define_arrow(edges, triangles, TRIANGLE_EDGE_90,       TRIANGLE_EDGE_45_LEFT,  arrow);
  } else if ( 5 == vertex_patterns) {
    return define_arrow(edges, triangles, TRIANGLE_EDGE_90,       TRIANGLE_EDGE_45_RIGHT, arrow);
  } else if ( 6 == vertex_patterns) {
    return define_arrow(edges, triangles, TRIANGLE_EDGE_45_RIGHT, TRIANGLE_EDGE_45_LEFT,  arrow);
  } else {
    // 0 / 7: all vertices are below / above the threshold
    // no arrows exist
    *arrow = NULL;
    return 0;
  }
}

