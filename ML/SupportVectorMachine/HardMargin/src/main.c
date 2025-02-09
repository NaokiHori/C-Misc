#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <math.h>
#include <errno.h>
#include "types.h"
#include "io.h"
#include "./prepare_dataset.h"
#include "./train.h"
#include "./predict.h"
#include "./line_segment.h"

#define NDIMS 2

static int output_training_dataset(
    const size_t nitems,
    const dataset_t * const items
) {
  const char * const file_names[] = {
    DATA_DIRECTORY "training_dataset_0.dat",
    DATA_DIRECTORY "training_dataset_1.dat",
  };
  errno = 0;
  FILE * const fps[] = {
    fopen(file_names[0], "w"),
    fopen(file_names[1], "w"),
  };
  if (NULL == fps[0]) {
    perror(file_names[0]);
    return 1;
  }
  if (NULL == fps[1]) {
    perror(file_names[1]);
    return 1;
  }
  for (size_t i = 0; i < nitems; i++) {
    const dataset_t * const item = items + i;
    const vector_t * const vector = &item->vector;
    const double label = item->label;
    fprintf(label < 0. ? fps[0] : fps[1], "% .7e % .7e\n", (*vector)[1], (*vector)[2]);
  }
  fclose(fps[0]);
  fclose(fps[1]);
  return 0;
}

static int output_support_vectors(
    const size_t nitems,
    const dataset_t * const items
) {
  const char file_name[] = DATA_DIRECTORY "support_vectors.dat";
  errno = 0;
  FILE * const fp = fopen(file_name, "w");
  if (NULL == fp) {
    perror(file_name);
    return 1;
  }
  for (size_t n = 0; n < nitems; n++) {
    const dataset_t * const item = items + n;
    if (!item->is_support_vector) {
      continue;
    }
    const vector_t * const vector = &item->vector;
    fprintf(fp, "% .7e % .7e\n", (*vector)[1], (*vector)[2]);
  }
  fclose(fp);
  return 0;
}

static int output_trained_boundary(
    const vector_t * const weights
) {
  size_t nitems = 0;
  vector_t intersections[2] = {{0}, {0}};
  if (0 != get_line_segment(weights, &nitems, intersections)) {
    return 1;
  }
  const char file_name[] = DATA_DIRECTORY "trained_boundary.dat";
  errno = 0;
  FILE * const fp = fopen(file_name, "w");
  if (NULL == fp) {
    perror(file_name);
    return 1;
  }
  for (size_t n = 0; n < nitems; n++) {
    fprintf(
        fp,
        "% .7e % .7e\n",
        intersections[n][1],
        intersections[n][2]
    );
  }
  fclose(fp);
  return 0;
}

int main(
    void
) {
  int error_code = 0;
  const size_t nitems = 1 << 8;
  const size_t n_training_items = nitems - (1 << 7);
  const size_t n_predicting_items = nitems - n_training_items;
  printf("%zu items in total\n", nitems);
  printf("%zu items to train\n", n_training_items);
  printf("%zu items to predict\n", n_predicting_items);
  dataset_t * const items = malloc(nitems * sizeof(dataset_t));
  vector_t weights = {0.};
  if (0 != prepare_dataset(nitems, items)) {
    error_code = 1;
    puts("failed to prepare a dataset");
    goto abort;
  }
  if (0 != output_training_dataset(n_training_items, items)) {
    error_code = 1;
    puts("failed to output a training dataset");
    goto abort;
  }
  if (0 != train(n_training_items, items, &weights)) {
    error_code = 1;
    puts("failed to train the dataset");
    goto abort;
  }
  if (0 != output_support_vectors(n_training_items, items)) {
    error_code = 1;
    puts("failed to output support vectors");
    goto abort;
  }
  if (0 != output_trained_boundary(&weights)) {
    error_code = 1;
    puts("failed to output the trained boundary");
    goto abort;
  }
  if (0 != predict(n_predicting_items, items + n_training_items, &weights)) {
    error_code = 1;
    puts("failed to predict the dataset");
    goto abort;
  }
abort:
  free(items);
  return error_code;
}

