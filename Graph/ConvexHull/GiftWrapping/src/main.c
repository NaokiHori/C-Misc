#include <errno.h>
#include <float.h>
#include <math.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct node_t {
  double x;
  double y;
} node_t;

typedef struct ch_item_t {
  const node_t * node;
  struct ch_item_t * next;
} ch_item_t;

static int normalize(
    double * const x,
    double * const y
) {
  const double x2 = pow(*x, 2.);
  const double y2 = pow(*y, 2.);
  double norm = sqrt(x2 + y2);
  norm = fmax(norm, DBL_EPSILON);
  *x /= norm;
  *y /= norm;
  return 0;
}

static int init_nodes(
    const size_t nnodes,
    node_t * const nodes
) {
  const double PI = 3.14159265358979324;
  for (size_t n = 0; n < nnodes; n++) {
    node_t * const node = nodes + n;
    const double r = 0.5 * rand() / RAND_MAX;
    const double t = 2. * PI * rand() / RAND_MAX;
    node->x = r * cos(t);
    node->y = r * sin(t);
  }
  return 0;
}

static int get_left_most_node(
    const size_t nnodes,
    node_t * const nodes,
    node_t ** const left_most_node
) {
  if (nnodes < 1) {
    printf("nnodes %zu should be positive\n", nnodes);
    return 1;
  }
  *left_most_node = nodes + 0;
  for (size_t n = 1; n < nnodes; n++) {
    node_t * const node = nodes + n;
    const double x = node->x;
    if (x < (*left_most_node)->x) {
      *left_most_node = node;
    }
  }
  return 0;
}

static int construct_convex_hull(
    const size_t nnodes,
    const node_t * const nodes,
    const node_t * const left_most_node,
    ch_item_t ** const ch_item
) {
  if (nnodes < 2) {
    printf("nnodes %zu should be more than 1\n", nnodes);
    return 1;
  }
  // n0 -> n1: base line segment
  // we set a dummy node positioned below the left-most node to launch
  node_t dummy_node = {
    .x = left_most_node->x,
    .y = left_most_node->y - 1.,
  };
  const node_t * n0 = &dummy_node;
  const node_t * n1 = left_most_node;
  while (1) {
    // initially consider a vertical vector
    double dx01 = n1->x - n0->x;
    double dy01 = n1->y - n0->y;
    normalize(&dx01, &dy01);
    // consider n1 -> n3, and check inner product with n0 -> n1
    // save a node giving the maximum dot product as n2:
    //   next node to n1 which also composes the convex hull
    const node_t * n2 = NULL;
    double max_dot = -1.;
    for (size_t n = 0; n < nnodes; n++) {
      const node_t * const n3 = nodes + n;
      if (n3 == n1) {
        continue;
      }
      double dx13 = n3->x - n1->x;
      double dy13 = n3->y - n1->y;
      normalize(&dx13, &dy13);
      const double dot = dx01 * dx13 + dy01 * dy13;
      if (max_dot < dot) {
        max_dot = dot;
        n2 = n3;
      }
    }
    ch_item_t * const new_ch_item = malloc(sizeof(ch_item_t));
    new_ch_item->node = n1;
    new_ch_item->next = *ch_item;
    *ch_item = new_ch_item;
    // move forward
    n0 = n1;
    n1 = n2;
    if (n1 == left_most_node) {
      break;
    }
  }
  return 0;
}

static int output_nodes(
    const size_t nnodes,
    node_t * const nodes
) {
  const char file_name[] = {"nodes.dat"};
  errno = 0;
  FILE * const fp = fopen(file_name, "w");
  if (NULL == fp) {
    perror(file_name);
    return 1;
  }
  for (size_t n = 0; n < nnodes; n++) {
    const node_t * const node = nodes + n;
    fprintf(fp, "% .15e % .15e\n", node->x, node->y);
  }
  fclose(fp);
  return 0;
}

static int output_convex_hull(
    const ch_item_t * ch_item
) {
  const char file_name[] = {"convex_hull.dat"};
  errno = 0;
  FILE * const fp = fopen(file_name, "w");
  if (NULL == fp) {
    perror(file_name);
    return 1;
  }
  while (ch_item) {
    const node_t * const node = ch_item->node;
    fprintf(fp, "% .15e % .15e\n", node->x, node->y);
    ch_item = ch_item->next;
  }
  fclose(fp);
  return 0;
}

int main(
    void
) {
  srand(1);
  // all nodes
  const size_t nnodes = 32;
  node_t * const nodes = malloc(nnodes * sizeof(node_t));
  // storing first item which composes the convex hull
  node_t * left_most_node = NULL;
  // singly-linked list to store the conex hull
  ch_item_t * ch_item = NULL;
  //
  init_nodes(nnodes, nodes);
  output_nodes(nnodes, nodes);
  if (0 != get_left_most_node(nnodes, nodes, &left_most_node)) {
    goto abort;
  }
  if (0 != construct_convex_hull(nnodes, nodes, left_most_node, &ch_item)) {
    goto abort;
  }
  output_convex_hull(ch_item);
abort:
  free(nodes);
  while (ch_item) {
    ch_item_t * const next = ch_item->next;
    free(ch_item);
    ch_item = next;
  }
  return 0;
}

