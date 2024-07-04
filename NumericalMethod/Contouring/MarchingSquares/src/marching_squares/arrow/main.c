#include <stddef.h> // size_t
#include <stdbool.h> // bool
#include <stdint.h> // uint8_t
#include <assert.h> // assert
#include "../internal.h"
#include "../memory.h"
#include "../arrow.h" // arrow_t, NMAXARROWS
#include "../edge.h" // edge_t, NEDGES

static arrow_t * define_arrow (
    const bool reverse,
    edge_t * const edges[NEDGES],
    cell_t * const cells[NEDGES],
    const edge_index_t tail,
    const edge_index_t head
) {
  assert(NULL != edges[tail]);
  assert(NULL != edges[head]);
  arrow_t * const arrow = memory_alloc(1 * sizeof(arrow_t));
  arrow->visited = false;
  arrow->edge_tail = reverse ? edges[head] : edges[tail];
  arrow->edge_head = reverse ? edges[tail] : edges[head];
  arrow->cell_tail = reverse ? cells[head] : cells[tail];
  arrow->cell_head = reverse ? cells[tail] : cells[head];
  return arrow;
}

int find_arrows_of_cell (
    const double threshold,
    const double vertex_values[NVERTICES],
    edge_t * const edges[NEDGES],
    cell_t * const cells[NEDGES],
    arrow_t * arrows[NMAXARROWS]
) {
  arrows[0] = NULL;
  arrows[1] = NULL;
  // 2 ^ 4 = 16 patterns, see figure
  uint8_t vertex_patterns = 0;
  for (size_t n = 0; n < NVERTICES; n++) {
    const uint8_t is_above = threshold < vertex_values[n] ? 1 : 0;
    vertex_patterns |= (1 & is_above) << n;
  }
  const bool reverse = 8 <= vertex_patterns;
  if (1 == vertex_patterns || 14 == vertex_patterns) {
    arrows[0] = define_arrow(reverse, edges, cells, EDGE_X_NEG, EDGE_Y_NEG);
    arrows[1] = NULL;
  } else if ( 2 == vertex_patterns || 13 == vertex_patterns) {
    arrows[0] = define_arrow(reverse, edges, cells, EDGE_Y_NEG, EDGE_X_POS);
    arrows[1] = NULL;
  } else if ( 3 == vertex_patterns || 12 == vertex_patterns) {
    arrows[0] = define_arrow(reverse, edges, cells, EDGE_X_NEG, EDGE_X_POS);
    arrows[1] = NULL;
  } else if ( 4 == vertex_patterns || 11 == vertex_patterns) {
    arrows[0] = define_arrow(reverse, edges, cells, EDGE_X_POS, EDGE_Y_POS);
    arrows[1] = NULL;
  } else if ( 5 == vertex_patterns || 10 == vertex_patterns) {
    // two arrows exist but ambiguous
    // use averaged value to decide
    const double ave_value = (
        + 0.25 * vertex_values[0]
        + 0.25 * vertex_values[1]
        + 0.25 * vertex_values[2]
        + 0.25 * vertex_values[3]
    );
    if (ave_value < threshold) {
      arrows[0] = define_arrow(reverse, edges, cells, EDGE_X_NEG, EDGE_Y_NEG);
      arrows[1] = define_arrow(reverse, edges, cells, EDGE_X_POS, EDGE_Y_POS);
    } else {
      arrows[0] = define_arrow(reverse, edges, cells, EDGE_X_POS, EDGE_Y_NEG);
      arrows[1] = define_arrow(reverse, edges, cells, EDGE_X_NEG, EDGE_Y_POS);
    }
  } else if ( 6 == vertex_patterns ||  9 == vertex_patterns) {
    arrows[0] = define_arrow(reverse, edges, cells, EDGE_Y_NEG, EDGE_Y_POS);
    arrows[1] = NULL;
  } else if ( 7 == vertex_patterns ||  8 == vertex_patterns) {
    arrows[0] = define_arrow(reverse, edges, cells, EDGE_X_NEG, EDGE_Y_POS);
    arrows[1] = NULL;
  } else {
    // 0 / 15: all vertices are below / above the threshold
    // no arrows exist
    arrows[0] = NULL;
    arrows[1] = NULL;
  }
  return 0;
}

