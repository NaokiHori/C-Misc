#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include <time.h>
#include "vector.h"
#include "io.h"
#include "./prepare_dataset.h"
#include "./line_segment.h"

int prepare_dataset(
    const size_t nitems,
    dataset_t * const items
) {
  srand(time(NULL));
  const vector_t weights = {
    -0.3,
    1. * rand() / (RAND_MAX - 1) - 0.5,
    1. * rand() / (RAND_MAX - 1) - 0.5,
  };
  // consider a linear function separating the entire space
  // NOTE: discard vectors which are too close to the boundary
  //         to ensure linearly-separable nature
  const double threshold = 5.e-2;
  for (size_t i = 0; i < nitems; i++) {
    dataset_t * const item = items + i;
    vector_t * const vector = &item->vector;
    item->is_support_vector = false;
generate:
    // specify a random position in [-1 : +1]
    // NOTE: the zero-th component serves as the intercept
    //         and it should be fixed to 1
    //         to be an augmented vector
    (*vector)[0] = 1.;
    (*vector)[1] = 2. * (1. * rand() / (RAND_MAX - 1) - 0.5);
    (*vector)[2] = 2. * (1. * rand() / (RAND_MAX - 1) - 0.5);
    const double signed_distance = vector_op.inner_product(&weights, vector) / sqrt(pow(weights[1], 2.) + pow(weights[2], 2.));
    if (fabs(signed_distance) < threshold) {
      goto generate;
    }
    if (0. < signed_distance) {
      item->label = 1.;
    } else {
      item->label = -1.;
    }
  }
  {
    size_t npoints = 0;
    vector_t intersections[2] = {{0}, {0}};
    if (0 == get_line_segment(&weights, &npoints, intersections)) {
      FILE * const fp = fopen(DATA_DIRECTORY "specified_boundary.dat", "w");
      for (size_t n = 0; n < npoints; n++) {
        fprintf(
            fp,
            "% .7e % .7e\n",
            intersections[n][1],
            intersections[n][2]
        );
      }
      fclose(fp);
    }
  }
  return 0;
}

