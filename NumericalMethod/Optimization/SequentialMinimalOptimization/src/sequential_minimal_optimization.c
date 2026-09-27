#include <float.h>
#include <math.h>
#include <stdbool.h>
#include <stdlib.h>
#include "sequential_minimal_optimization.h"

static const size_t MAX_ITERATIONS = 32768;
static const double C = 1.e+3;
static const double MULTIPLER_TOLERANCE = 1.e-8;

static bool is_support_vector(
    const double multipler
) {
  return MULTIPLER_TOLERANCE < multipler;
}

static int choose_indices(
    const size_t nitems,
    const sample_t * const samples,
    const double * const kernel,
    bool * const indices_exist,
    size_t (* const indices)[2],
    double * const multipliers,
    double * const residual
) {
  const double tolerance = 1.e-8;
  bool lower_exists = false;
  bool upper_exists = false;
  double lower_most_value = - INFINITY;
  double upper_most_value = + INFINITY;
  size_t * const lower_most_index = *indices + 0;
  size_t * const upper_most_index = *indices + 1;
  for (size_t i = 0; i < nitems; i++) {
    const sample_t * const sample = samples + i;
    const double multipler = multipliers[i];
    // value = category * gradient
    double value = sample->category;
    for (size_t j = 0; j < nitems; j++) {
      const sample_t * const sample_j = samples + j;
      const double multipler_j = multipliers[j];
      value -= multipler_j * sample_j->category * kernel[i * nitems + j];
    }
    const bool is_lower = (
        sample->category == POSITIVE
        &&
        multipler < C - MULTIPLER_TOLERANCE
    ) || (
        sample->category == NEGATIVE
        &&
        MULTIPLER_TOLERANCE < multipler
    );
    const bool is_upper = (
        sample->category == POSITIVE
        &&
        MULTIPLER_TOLERANCE < multipler
    ) || (
        sample->category == NEGATIVE
        &&
        multipler < C - MULTIPLER_TOLERANCE
    );
    if (is_lower) {
      lower_exists = true;
      if (lower_most_value < value) {
        lower_most_value = value;
        *lower_most_index = i;
      }
    }
    if (is_upper) {
      upper_exists = true;
      if (value < upper_most_value) {
        upper_most_value = value;
        *upper_most_index = i;
      }
    }
  }
  if (!(lower_exists && upper_exists)) {
    *indices_exist = false;
    return 0;
  }
  // update multipler of equality constraint
  multipliers[nitems] = 0.5 * lower_most_value + 0.5 * upper_most_value;
  *residual = lower_most_value - upper_most_value;
  *indices_exist = tolerance < *residual;
  return 0;
}

static int compute_sum(
    const size_t nitems,
    const sample_t * const samples,
    const double * const kernel,
    const size_t (* const indices)[2],
    const double * const multipliers,
    double * const sum0,
    double * const sum1
) {
  const size_t index0 = (*indices)[0];
  const size_t index1 = (*indices)[1];
  *sum0 = 0.;
  *sum1 = 0.;
  for (size_t i = 0; i < nitems; i++) {
    if (index0 == i || index1 == i) {
      continue;
    }
    const sample_t * const sample = samples + i;
    const double multipler = multipliers[i];
    *sum0 += multipler * sample->category * kernel[i * nitems + index0];
    *sum1 += multipler * sample->category * kernel[i * nitems + index1];
  }
  return 0;
}

static int update_multipliers(
    const size_t nitems,
    const sample_t * const samples,
    const double * const kernel,
    const size_t (* const indices)[2],
    double * const multipliers
) {
  const size_t index0 = (*indices)[0];
  const size_t index1 = (*indices)[1];
  const sample_t * const sample0 = samples + index0;
  const sample_t * const sample1 = samples + index1;
  const category_t category0 = sample0->category;
  const category_t category1 = sample1->category;
  double * const multipler0 = multipliers + index0;
  double * const multipler1 = multipliers + index1;
  // update two multipliers while keeping this total
  const double total = category0 * *multipler0 + category1 * *multipler1;
  const double k00 = kernel[index0 * nitems + index0];
  const double k01 = kernel[index0 * nitems + index1];
  const double k11 = kernel[index1 * nitems + index1];
  const double denominator = 2. * k01 - k00 - k11;
  if (fabs(denominator) < DBL_EPSILON) {
    // two samples are too close
    return 0;
  }
  double sum0 = 0.;
  double sum1 = 0.;
  compute_sum(nitems, samples, kernel, indices, multipliers, &sum0, &sum1);
  *multipler0 = (
      category0 * category1
      - 1.
      + total * category0 * (k01 - k11)
      + category0 * (sum0 - sum1)
  ) / denominator;
  const double lower = category0 == category1
    ? fmax(0., total * category0 - C)
    : fmax(0., total * category0);
  const double upper = category0 == category1
    ? fmin(C, total * category0)
    : fmin(C, C + total * category0);
  *multipler0 = fmax(*multipler0, lower);
  *multipler0 = fmin(*multipler0, upper);
  *multipler1 = category1 * (total - category0 * *multipler0);
  return 0;
}

int categorizer_initialize(
    const size_t nitems,
    double (* const compute_kernel)(
        const double (* const at0)[NDIMS],
        const double (* const at1)[NDIMS]
    ),
    categorizer_t * const categorizer
) {
  categorizer->nitems = nitems;
  categorizer->compute_kernel = compute_kernel;
  double ** const multipliers = &categorizer->multipliers;
  sample_t ** const samples = &categorizer->samples;
  size_t ** const support_vector_indices = &categorizer->support_vector_indices;
  double ** const kernel = &categorizer->kernel;
  *multipliers = malloc((nitems + 1) * sizeof(double));
  *samples = malloc(nitems * sizeof(sample_t));
  *support_vector_indices = malloc(nitems * sizeof(size_t));
  *kernel = malloc(nitems * nitems * sizeof(double));
  if (
      NULL == *multipliers
      || NULL == *samples
      || NULL == *support_vector_indices
      || NULL == *kernel
  ) {
    return 1;
  }
  for (size_t i = 0; i < nitems + 1; i++) {
    (*multipliers)[i] = 0.;
  }
  for (size_t i = 0; i < nitems; i++) {
    (*support_vector_indices)[i] = 0;
  }
  for (size_t i = 0; i < nitems; i++) {
    for (size_t j = 0; j < nitems; j++) {
      (*kernel)[i * nitems + j] = 0.;
    }
  }
  return 0;
}

int categorizer_finalize(
    categorizer_t * const categorizer
) {
  free(categorizer->multipliers);
  free(categorizer->samples);
  free(categorizer->support_vector_indices);
  return 0;
}

int categorizer_optimize(
    categorizer_t * const categorizer,
    double * const residual
) {
  const size_t nitems = categorizer->nitems;
  const sample_t * const samples = categorizer->samples;
  double * const multipliers = categorizer->multipliers;
  double * const kernel = categorizer->kernel;
  double (* const compute_kernel)(
      const double (* const at0)[NDIMS],
      const double (* const at1)[NDIMS]
  ) = categorizer->compute_kernel;
  for (size_t i = 0; i < nitems; i++) {
    const double (* const at_i)[NDIMS] = &samples[i].at;
    for (size_t j = 0; j < nitems; j++) {
      const double (* const at_j)[NDIMS] = &samples[j].at;
      kernel[i * nitems + j] = compute_kernel(at_i, at_j);
    }
  }
  for (size_t iteration = 0; iteration < MAX_ITERATIONS; iteration++) {
    bool indices_exist = false;
    size_t indices[2] = {0, 0};
    choose_indices(nitems, samples, kernel, &indices_exist, &indices, multipliers, residual);
    if (!indices_exist) {
      break;
    }
    update_multipliers(nitems, samples, kernel, &indices, multipliers);
  }
  // list-up support vectors for later categorization
  size_t * const n_support_vectors = &categorizer->n_support_vectors;
  size_t * const support_vector_indices = categorizer->support_vector_indices;
  *n_support_vectors = 0;
  for (size_t i = 0; i < nitems; i++) {
    const double multipler = multipliers[i];
    if (is_support_vector(multipler)) {
      support_vector_indices[*n_support_vectors] = i;
      *n_support_vectors += 1;
    }
  }
  return 0;
}

int categorizer_categorize(
    const categorizer_t * const categorizer,
    const double (* const at)[NDIMS],
    category_t * const category
) {
  const size_t nitems = categorizer->nitems;
  double (* const compute_kernel)(
      const double (* const at0)[NDIMS],
      const double (* const at1)[NDIMS]
  ) = categorizer->compute_kernel;
  const size_t n_support_vectors = categorizer->n_support_vectors;
  const size_t * const support_vector_indices = categorizer->support_vector_indices;
  const sample_t * const samples = categorizer->samples;
  const double * const multipliers = categorizer->multipliers;
  double value = multipliers[nitems];
  for (size_t i = 0; i < n_support_vectors; i++) {
    const size_t j = support_vector_indices[i];
    const sample_t * const sample = samples + j;
    const double multipler = multipliers[j];
    value += multipler * sample->category * compute_kernel(at, &sample->at);
  }
  *category = value < 0. ? NEGATIVE : POSITIVE;
  return 0;
}

