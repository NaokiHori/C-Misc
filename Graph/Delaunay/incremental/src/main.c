#include <errno.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include "delaunay.h"

static int output_points(
    const size_t num_points,
    const point_t * const points
) {
  const char file_name[] = {"points.dat"};
  FILE * const fp = fopen(file_name, "w");
  if (fp == NULL) {
    perror(file_name);
    return 1;
  }
  for (size_t i = 0; i < num_points; i++) {
    const point_t * const point = points + i;
    fprintf(fp, "% .15e % .15e\n", point->x, point->y);
  }
  fclose(fp);
  return 0;
}

static int output_triangles(
    const triangle_node_t * triangle_node
) {
  const char file_name[] = {"triangles.dat"};
  errno = 0;
  FILE * const fp = fopen(file_name, "w");
  if (NULL == fp) {
    perror(file_name);
    return 1;
  }
  while (triangle_node) {
    const triangle_t * const triangle = &triangle_node->triangle;
    for (size_t i = 0; i < TRIANGLE + 1; i++) {
      const point_t * const point = (*triangle)[i % TRIANGLE];
      fprintf(fp, "% .15e % .15e\n", point->x, point->y);
    }
    fputs("\n", fp);
    triangle_node = triangle_node->next;
  }
  fclose(fp);
  return 0;
}

int main(
    void
) {
  srand(1);
  // sample points
  const size_t num_points = 32;
  point_t * const points = malloc(num_points * sizeof(point_t));
  for (size_t i = 0; i < num_points; i++) {
    point_t * const point = points + i;
    point->x = 1. * rand() / RAND_MAX;
    point->y = 1. * rand() / RAND_MAX;
  }
  if (0 != output_points(num_points, points)) {
    return 1;
  }
  // triangulate given points
  triangle_node_t * triangle_list_head = NULL;
  if (0 != delaunay_triangulate(num_points, points, &triangle_list_head)) {
    return 1;
  }
  output_triangles(triangle_list_head);
  // check
  if (0 != delaunay_check(num_points, points, triangle_list_head)) {
    return 1;
  }
  // clean-up resources
  while (triangle_list_head) {
    triangle_node_t * const next = triangle_list_head->next;
    free(triangle_list_head);
    triangle_list_head = next;
  }
  free(points);
  return 0;
}

