#include <stdio.h>
#include <errno.h>
#include "vector.h"
#include "./predict.h"
#include "./io.h"

int predict(
    const size_t nitems,
    const dataset_t * const items,
    const vector_t * const weights
) {
  const char * const file_names[] = {
    DATA_DIRECTORY "predicted_dataset_0.dat",
    DATA_DIRECTORY "predicted_dataset_1.dat",
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
  for (size_t n = 0; n < nitems; n++) {
    const dataset_t * const item = items + n;
    const vector_t * const vector = &item->vector;
    const double predicted = vector_op.inner_product(weights, vector);
    fprintf(predicted < 0. ? fps[0] : fps[1], "% .1e % .1e\n", (*vector)[1], (*vector)[2]);
    if (predicted * item->label < 0.) {
      printf("wrongly predicted: % .7e % .7e\n", (*vector)[1], (*vector)[2]);
    }
  }
  fclose(fps[0]);
  fclose(fps[1]);
  return 0;
}

