#include <errno.h>
#include <math.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include "sequential_minimal_optimization.h"

#define SAMPLE_TYPE_CIRCLE 0
#define SAMPLE_TYPE_LINE 1
#define SAMPLE_TYPE SAMPLE_TYPE_CIRCLE

#if SAMPLE_TYPE == SAMPLE_TYPE_CIRCLE
static double compute_kernel(
    const double (* const at0)[NDIMS],
    const double (* const at1)[NDIMS]
) {
  const double gamma = 5.e+0;
  double inner_product = 0.;
  for (size_t dim = 0; dim < NDIMS; dim++) {
    inner_product += pow((*at1)[dim] - (*at0)[dim], 2.);
  }
  return exp(- gamma * inner_product);
}
#elif SAMPLE_TYPE == SAMPLE_TYPE_LINE
static double compute_kernel(
    const double (* const at0)[NDIMS],
    const double (* const at1)[NDIMS]
) {
  return (*at0)[0] * (*at1)[0] + (*at0)[1] * (*at1)[1];
}
#else
#error "unknown SAMPLE_TYPE"
#endif

static int output_sample(
    FILE * const fp,
    const double (* const at)[NDIMS],
    const category_t * const category
) {
  for (size_t dim = 0; dim < NDIMS; dim++) {
    fprintf(fp, "% .15e ", (*at)[dim]);
  }
  fprintf(fp, "%d\n", *category);
  return 0;
}

static int setup_samples(
    categorizer_t * const categorizer
) {
  const size_t nitems = categorizer->nitems;
  sample_t * const samples = categorizer->samples;
  for (size_t i = 0; i < nitems; i++) {
    sample_t * const sample = samples + i;
    double (* const at)[NDIMS] = &sample->at;
    category_t * const category = &sample->category;
    // x_i in [-1 : 1]
    for (size_t dim = 0; dim < NDIMS; dim++) {
      (*at)[dim] = 2. * (-0.5 + 1. * rand() / RAND_MAX);
    }
#if SAMPLE_TYPE == SAMPLE_TYPE_CIRCLE
    const double center[NDIMS] = {0.125, 0.25};
    double squared_distance = 0.;
    for (size_t dim = 0; dim < NDIMS; dim++) {
      squared_distance += pow((*at)[dim] - center[dim], 2.);
    }
    const double radius = 0.5;
    *category = squared_distance < pow(radius, 2.) ? NEGATIVE : POSITIVE;
#elif SAMPLE_TYPE == SAMPLE_TYPE_LINE
    *category = - (*at)[0] - (*at)[1] < 0. ? NEGATIVE : POSITIVE;
#else
#error "unknown SAMPLE_TYPE"
#endif
  }
  return 0;
}

static int output_samples(
    categorizer_t * const categorizer
) {
  const char file_name[] = "training_set.dat";
  errno = 0;
  FILE * const fp = fopen(file_name, "w");
  if (NULL == fp) {
    perror(file_name);
    return 1;
  }
  const size_t nitems = categorizer->nitems;
  sample_t * const samples = categorizer->samples;
  const size_t n_support_vectors = categorizer->n_support_vectors;
  const size_t * const support_vector_indices = categorizer->support_vector_indices;
  // output support vectors first, followed by other samples
  for (size_t i = 0; i < nitems; i++) {
    const sample_t * const sample = samples + i;
    for (size_t j = 0; j < n_support_vectors; j++) {
      if (support_vector_indices[j] == i) {
        output_sample(fp, &sample->at, &sample->category);
        break;
      }
    }
  }
  fprintf(fp, "\n");
  for (size_t i = 0; i < nitems; i++) {
    const sample_t * const sample = samples + i;
    bool is_support_vector = false;
    for (size_t j = 0; j < n_support_vectors; j++) {
      if (support_vector_indices[j] == i) {
        is_support_vector = true;
        break;
      }
    }
    if (!is_support_vector) {
      output_sample(fp, &sample->at, &sample->category);
    }
  }
  fclose(fp);
  return 0;
}

static int categorize_others(
    categorizer_t * const categorizer
) {
  const char file_name[] = "test_set.dat";
  errno = 0;
  FILE * const fp = fopen(file_name, "w");
  if (NULL == fp) {
    perror(file_name);
    return 1;
  }
  const size_t nitems = 64;
  for (size_t i = 0; i < nitems; i++) {
    const double x = 2. * (0.5 * (2 * i + 1) / nitems) - 1.;
    for (size_t j = 0; j < nitems; j++) {
      const double y = 2. * (0.5 * (2 * j + 1) / nitems) - 1.;
      const double at[NDIMS] = {x, y};
      category_t category = POSITIVE;
      if (0 != categorizer_categorize(categorizer, &at, &category)) {
        return 1;
      }
      output_sample(fp, &at, &category);
    }
  }
  fclose(fp);
  return 0;
}

int main(
    void
) {
  srand(1);
  const size_t nitems = 256;
  categorizer_t categorizer = {0};
  if (0 != categorizer_initialize(nitems, compute_kernel, &categorizer)) {
    return 1;
  }
  // assign sample position and category
  setup_samples(&categorizer);
  // run optimizer
  puts("start optimizing (may take seconds) ...");
  double residual = 0.;
  if (0 != categorizer_optimize(&categorizer, &residual)) {
    return 1;
  }
  printf(
      "optimization done; number of support vectors: %zu, residual: % .1e\n",
      categorizer.n_support_vectors,
      residual
  );
  output_samples(&categorizer);
  // categorize other samples
  categorize_others(&categorizer);
  if (0 != categorizer_finalize(&categorizer)) {
    return 1;
  }
  return 0;
}

