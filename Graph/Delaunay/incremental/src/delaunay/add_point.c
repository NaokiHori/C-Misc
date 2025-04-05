#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include "./add_point.h"
#include "./circumcircle.h"

typedef const point_t * edge_t[2];

typedef struct edge_node_t {
  const edge_t edge;
  struct edge_node_t * next;
} edge_node_t;

static int find_bad_triangles(
    const point_t * const point,
    triangle_node_t ** triangle_node,
    triangle_node_t ** const bad_triangle_list_head
) {
  while (*triangle_node) {
    const triangle_t * const triangle = &(*triangle_node)->triangle;
    if (point_is_in_circumcircle(point, triangle)) {
      // move node from triangle list to bad triangle list
      triangle_node_t * const moved_node = *triangle_node;
      triangle_node_t * const next_node = (*triangle_node)->next;
      *triangle_node = next_node;
      moved_node->next = *bad_triangle_list_head;
      *bad_triangle_list_head = moved_node;
    } else {
      triangle_node = &(*triangle_node)->next;
    }
  }
  return 0;
}

static bool two_edges_overlap(
    const edge_t * const ea,
    const edge_t * const eb
) {
  if ((*ea)[0] == (*eb)[0] && (*ea)[1] == (*eb)[1]) {
    return true;
  }
  if ((*ea)[0] == (*eb)[1] && (*ea)[1] == (*eb)[0]) {
    return true;
  }
  return false;
}

static int add_edge_to_list(
    const edge_t * const edge,
    edge_node_t ** edge_node
) {
  // iterate over the existing edges
  //   if found, remove the existing edge as that is non-unique
  //   if not found, add it to the list
  while (*edge_node) {
    if (two_edges_overlap(&(*edge_node)->edge, edge)) {
      // NOTE: it is out of consideration when more than two triangles share the same edge
      //       such circumstance indicates the algorithm breaks down in an earlier stage and thus we omit it
      edge_node_t * const next = (*edge_node)->next;
      free(*edge_node);
      *edge_node = next;
      return 0;
    }
    edge_node = &(*edge_node)->next;
  }
  // reaching here means that this edge is found for the first time
  // add it to the end of the list
  edge_node_t * const new_edge_node = malloc(sizeof(edge_node_t));
  memcpy(
      new_edge_node,
      &(edge_node_t){
        .edge = {
          (*edge)[0],
          (*edge)[1],
        },
        .next = NULL,
      },
      sizeof(edge_node_t)
  );
  *edge_node = new_edge_node;
  return 0;
}

/**
 * Given a list of triangles, find a list of edges which are unique
 */
static int find_unique_edges(
    triangle_node_t ** triangle_list_head,
    edge_node_t ** const edge_list_head
) {
  triangle_node_t * triangle_node = *triangle_list_head;
  while (triangle_node) {
    const triangle_t * const triangle = &triangle_node->triangle;
    for (size_t i = 0; i < TRIANGLE; i++) {
      const edge_t triangle_edge = {
        (*triangle)[(i + 0) % TRIANGLE],
        (*triangle)[(i + 1) % TRIANGLE],
      };
      add_edge_to_list(&triangle_edge, edge_list_head);
    }
    triangle_node = triangle_node->next;
  }
  // Clean-up list of bad triangles,
  //   which will no longer be used
  while (*triangle_list_head) {
    triangle_node_t * const next = (*triangle_list_head)->next;
    free(*triangle_list_head);
    *triangle_list_head = next;
  }
  return 0;
}

static int create_triangles(
    const point_t * const point,
    edge_node_t ** edge_list_head,
    triangle_node_t ** triangle_list_head
) {
  while (*edge_list_head) {
    const edge_t * const edge = &(*edge_list_head)->edge;
    triangle_node_t * const new_triangle_node = malloc(sizeof(triangle_node_t));
    memcpy(
        new_triangle_node,
        &(triangle_node_t){
          .triangle = {
            (*edge)[0],
            (*edge)[1],
            point,
          },
          .next = *triangle_list_head,
        },
        sizeof(triangle_node_t)
    );
    *triangle_list_head = new_triangle_node;
    edge_node_t * const next = (*edge_list_head)->next;
    free(*edge_list_head);
    *edge_list_head = next;
  }
  return 0;
}

int add_point(
    const point_t * const point,
    triangle_node_t ** const triangle_list_head
) {
  // find "bad triangles", which have the given point
  //   inside their circumcircles
  triangle_node_t * bad_triangle_list_head = NULL;
  if (0 != find_bad_triangles(point, triangle_list_head, &bad_triangle_list_head)) {
    return 1;
  }
  // find new edges which consist of new triangles
  edge_node_t * edge_list_head = NULL;
  if (0 != find_unique_edges(&bad_triangle_list_head, &edge_list_head)) {
    return 1;
  }
  // create new triangles using the point of interest
  //   and the edges constructing the polygonal hole
  if (0 != create_triangles(point, &edge_list_head, triangle_list_head)) {
    return 1;
  }
  return 0;
}

