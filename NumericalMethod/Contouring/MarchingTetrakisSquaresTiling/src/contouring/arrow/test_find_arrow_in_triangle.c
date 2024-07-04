#if defined(TEST_MARCHING_SQUARE)

#include <stdio.h>
#include <stdlib.h>
#include "../internal.h"
#include "../arrow.h"
#include "../edge.h"
#include "../triangle.h"

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
    arrow_t * result = NULL; \
    triangle_t * triangles[NUM_TRIANGLE_EDGES] = {NULL, NULL, NULL}; \
    find_arrow_in_triangle(threshold, vertex_values, (const edge_t * const *)edges, triangles, &result); \
    const int retval = report(__func__, expect, result); \
    if (0 == retval) { \
      printf("%-10s passed\n", __func__); \
    } \
    return retval; \
  } while (0)

static const double threshold = 0.5;

static int test0 (
    edge_t * const edges[NUM_TRIANGLE_EDGES]
) {
  const double vertex_values[] = {0., 0., 0.};
  arrow_t * expect = NULL;
  CHECK();
}

static int test1 (
    edge_t * const edges[NUM_TRIANGLE_EDGES]
) {
  const double vertex_values[] = {1., 0., 0.};
  arrow_t * expect = &(arrow_t){
    .edge_tail = edges[TRIANGLE_EDGE_45_LEFT ],
    .edge_head = edges[TRIANGLE_EDGE_45_RIGHT],
  };
  CHECK();
}

static int test2 (
    edge_t * const edges[NUM_TRIANGLE_EDGES]
) {
  const double vertex_values[] = {0., 1., 0.};
  arrow_t * expect = &(arrow_t){
    .edge_tail = edges[TRIANGLE_EDGE_45_RIGHT],
    .edge_head = edges[TRIANGLE_EDGE_90      ],
  };
  CHECK();
}

static int test3 (
    edge_t * const edges[NUM_TRIANGLE_EDGES]
) {
  const double vertex_values[] = {1., 1., 0.};
  arrow_t * expect = &(arrow_t){
    .edge_tail = edges[TRIANGLE_EDGE_45_LEFT ],
    .edge_head = edges[TRIANGLE_EDGE_90      ],
  };
  CHECK();
}

static int test4 (
    edge_t * const edges[NUM_TRIANGLE_EDGES]
) {
  const double vertex_values[] = {0., 0., 1.};
  arrow_t * expect = &(arrow_t){
    .edge_tail = edges[TRIANGLE_EDGE_90      ],
    .edge_head = edges[TRIANGLE_EDGE_45_LEFT ],
  };
  CHECK();
}

static int test5 (
    edge_t * const edges[NUM_TRIANGLE_EDGES]
) {
  const double vertex_values[] = {1., 0., 1.};
  arrow_t * expect = &(arrow_t){
    .edge_tail = edges[TRIANGLE_EDGE_90      ],
    .edge_head = edges[TRIANGLE_EDGE_45_RIGHT],
  };
  CHECK();
}

static int test6 (
    edge_t * const edges[NUM_TRIANGLE_EDGES]
) {
  const double vertex_values[] = {0., 1., 1.};
  arrow_t * expect = &(arrow_t){
    .edge_tail = edges[TRIANGLE_EDGE_45_RIGHT],
    .edge_head = edges[TRIANGLE_EDGE_45_LEFT ],
  };
  CHECK();
}

static int test7 (
    edge_t * const edges[NUM_TRIANGLE_EDGES]
) {
  const double vertex_values[] = {1., 1., 1.};
  arrow_t * expect = NULL;
  CHECK();
}

int main (
    void
) {
  int retval = 0;
  edge_t * const edges[NUM_TRIANGLE_EDGES] = {
    malloc(1 * sizeof(edge_t)),
    malloc(1 * sizeof(edge_t)),
    malloc(1 * sizeof(edge_t)),
  };
  retval += test0(edges);
  retval += test1(edges);
  retval += test2(edges);
  retval += test3(edges);
  retval += test4(edges);
  retval += test5(edges);
  retval += test6(edges);
  retval += test7(edges);
  for (size_t n = 0; n < NUM_TRIANGLE_EDGES; n++) {
    free(edges[n]);
  }
  return retval;
}

#else

extern char dummy;

#endif
