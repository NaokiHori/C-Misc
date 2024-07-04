#if defined(TEST_MARCHING_SQUARE)

#include <stdio.h>
#include <stdlib.h>
#include "../internal.h"
#include "../arrow.h"
#include "../edge.h" // NEDGES

static int report (
    const char func[],
    const arrow_t * const expect,
    const arrow_t * const result
) {
  // OK: both NULL
  if (NULL == expect && NULL == result) {
    return 0;
  }
  // NG: one is NULL while the other one is not
  if ((NULL == expect && NULL != result) || (NULL != expect && NULL == result)) {
    printf("%-10s failed, expect %p, result %p\n", func, expect, result);
    return 1;
  }
  // both non-NULL, investigate members
  int retval = 0;
  if (expect->edge_tail != result->edge_tail) {
    printf("%-10s failed, expect->edge_tail %p, result->edge_tail %p\n", func, expect->edge_tail, result->edge_tail);
    retval += 1;
  }
  if (expect->edge_head != result->edge_head) {
    printf("%-10s failed, expect->edge_head %p, result->edge_head %p\n", func, expect->edge_head, result->edge_head);
    retval += 1;
  }
  return retval;
}

#define CHECK() \
  do { \
    arrow_t * result[2] = {NULL, NULL}; \
    cell_t * cells[NEDGES] = {NULL, NULL, NULL, NULL}; \
    find_arrows_of_cell(threshold, vertex_values, edges, cells, result); \
    const int retval = \
      + report(__func__, expect[0], result[0]) \
      + report(__func__, expect[1], result[1]); \
    if (0 == retval) { \
      printf("%-10s passed\n", __func__); \
    } \
    return retval; \
  } while (0)

static const double threshold = 0.5;

static int test00 (
    edge_t * const edges[NEDGES]
) {
  const double vertex_values[] = {0., 0., 0., 0.};
  arrow_t * expect[2] = {NULL, NULL};
  CHECK();
}

static int test01 (
    edge_t * const edges[NEDGES]
) {
  const double vertex_values[] = {1., 0., 0., 0.};
  arrow_t * expect[2] = {
    &(arrow_t){
      .edge_tail = edges[EDGE_X_NEG],
      .edge_head = edges[EDGE_Y_NEG],
    },
    NULL,
  };
  CHECK();
}

static int test02 (
    edge_t * const edges[NEDGES]
) {
  const double vertex_values[] = {0., 1., 0., 0.};
  arrow_t * expect[2] = {
    &(arrow_t){
      .edge_tail = edges[EDGE_Y_NEG],
      .edge_head = edges[EDGE_X_POS],
    },
    NULL,
  };
  CHECK();
}

static int test03 (
    edge_t * const edges[NEDGES]
) {
  const double vertex_values[] = {1., 1., 0., 0.};
  arrow_t * expect[2] = {
    &(arrow_t){
      .edge_tail = edges[EDGE_X_NEG],
      .edge_head = edges[EDGE_X_POS],
    },
    NULL,
  };
  CHECK();
}

static int test04 (
    edge_t * const edges[NEDGES]
) {
  const double vertex_values[] = {0., 0., 1., 0.};
  arrow_t * expect[2] = {
    &(arrow_t){
      .edge_tail = edges[EDGE_X_POS],
      .edge_head = edges[EDGE_Y_POS],
    },
    NULL,
  };
  CHECK();
}

static int test05_a (
    edge_t * const edges[NEDGES]
) {
  const double vertex_values[] = {0.75, 0., 0.75, 0.};
  arrow_t * expect[2] = {
    &(arrow_t){
      .edge_tail = edges[EDGE_X_NEG],
      .edge_head = edges[EDGE_Y_NEG],
    },
    &(arrow_t){
      .edge_tail = edges[EDGE_X_POS],
      .edge_head = edges[EDGE_Y_POS],
    },
  };
  CHECK();
}

static int test05_b (
    edge_t * const edges[NEDGES]
) {
  const double vertex_values[] = {1.25, 0., 1.25, 0.};
  arrow_t * expect[2] = {
    &(arrow_t){
      .edge_tail = edges[EDGE_X_POS],
      .edge_head = edges[EDGE_Y_NEG],
    },
    &(arrow_t){
      .edge_tail = edges[EDGE_X_NEG],
      .edge_head = edges[EDGE_Y_POS],
    },
  };
  CHECK();
}

static int test06 (
    edge_t * const edges[NEDGES]
) {
  const double vertex_values[] = {0., 1., 1., 0.};
  arrow_t * expect[2] = {
    &(arrow_t){
      .edge_tail = edges[EDGE_Y_NEG],
      .edge_head = edges[EDGE_Y_POS],
    },
    NULL,
  };
  CHECK();
}

static int test07 (
    edge_t * const edges[NEDGES]
) {
  const double vertex_values[] = {1., 1., 1., 0.};
  arrow_t * expect[2] = {
    &(arrow_t){
      .edge_tail = edges[EDGE_X_NEG],
      .edge_head = edges[EDGE_Y_POS],
    },
    NULL,
  };
  CHECK();
}

static int test08 (
    edge_t * const edges[NEDGES]
) {
  const double vertex_values[] = {0., 0., 0., 1.};
  arrow_t * expect[2] = {
    &(arrow_t){
      .edge_tail = edges[EDGE_Y_POS],
      .edge_head = edges[EDGE_X_NEG],
    },
    NULL,
  };
  CHECK();
}

static int test09 (
    edge_t * const edges[NEDGES]
) {
  const double vertex_values[] = {1., 0., 0., 1.};
  arrow_t * expect[2] = {
    &(arrow_t){
      .edge_tail = edges[EDGE_Y_POS],
      .edge_head = edges[EDGE_Y_NEG],
    },
    NULL,
  };
  CHECK();
}

static int test10_a (
    edge_t * const edges[NEDGES]
) {
  const double vertex_values[] = {0., 0.75, 0., 0.75};
  arrow_t * expect[2] = {
    &(arrow_t){
      .edge_tail = edges[EDGE_Y_NEG],
      .edge_head = edges[EDGE_X_NEG],
    },
    &(arrow_t){
      .edge_tail = edges[EDGE_Y_POS],
      .edge_head = edges[EDGE_X_POS],
    },
  };
  CHECK();
}

static int test10_b (
    edge_t * const edges[NEDGES]
) {
  const double vertex_values[] = {0., 1.25, 0., 1.25};
  arrow_t * expect[2] = {
    &(arrow_t){
      .edge_tail = edges[EDGE_Y_NEG],
      .edge_head = edges[EDGE_X_POS],
    },
    &(arrow_t){
      .edge_tail = edges[EDGE_Y_POS],
      .edge_head = edges[EDGE_X_NEG],
    },
  };
  CHECK();
}

static int test11 (
    edge_t * const edges[NEDGES]
) {
  const double vertex_values[] = {1., 1., 0., 1.};
  arrow_t * expect[2] = {
    &(arrow_t){
      .edge_tail = edges[EDGE_Y_POS],
      .edge_head = edges[EDGE_X_POS],
    },
    NULL,
  };
  CHECK();
}

static int test12 (
    edge_t * const edges[NEDGES]
) {
  const double vertex_values[] = {0., 0., 1., 1.};
  arrow_t * expect[2] = {
    &(arrow_t){
      .edge_tail = edges[EDGE_X_POS],
      .edge_head = edges[EDGE_X_NEG],
    },
    NULL,
  };
  CHECK();
}

static int test13 (
    edge_t * const edges[NEDGES]
) {
  const double vertex_values[] = {1., 0., 1., 1.};
  arrow_t * expect[2] = {
    &(arrow_t){
      .edge_tail = edges[EDGE_X_POS],
      .edge_head = edges[EDGE_Y_NEG],
    },
    NULL,
  };
  CHECK();
}

static int test14 (
    edge_t * const edges[NEDGES]
) {
  const double vertex_values[] = {0., 1., 1., 1.};
  arrow_t * expect[2] = {
    &(arrow_t){
      .edge_tail = edges[EDGE_Y_NEG],
      .edge_head = edges[EDGE_X_NEG],
    },
    NULL,
  };
  CHECK();
}

static int test15 (
    edge_t * const edges[NEDGES]
) {
  const double vertex_values[] = {1., 1., 1., 1.};
  arrow_t * expect[2] = {NULL, NULL};
  CHECK();
}

int main (
    void
) {
  int retval = 0;
  edge_t * const edges[NEDGES] = {
    malloc(1 * sizeof(edge_t)),
    malloc(1 * sizeof(edge_t)),
    malloc(1 * sizeof(edge_t)),
    malloc(1 * sizeof(edge_t)),
  };
  retval += test00  (edges);
  retval += test01  (edges);
  retval += test02  (edges);
  retval += test03  (edges);
  retval += test04  (edges);
  retval += test05_a(edges);
  retval += test05_b(edges);
  retval += test06  (edges);
  retval += test07  (edges);
  retval += test08  (edges);
  retval += test09  (edges);
  retval += test10_a(edges);
  retval += test10_b(edges);
  retval += test11  (edges);
  retval += test12  (edges);
  retval += test13  (edges);
  retval += test14  (edges);
  retval += test15  (edges);
  free(edges[0]);
  free(edges[1]);
  free(edges[2]);
  free(edges[3]);
  return retval;
}

#else

extern char dummy;

#endif
