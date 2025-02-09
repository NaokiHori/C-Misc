#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include <float.h>
#include "vector.h"
#include "io.h"
#include "./train.h"

static const struct {
  bool correct_negative_alpha;
  bool correct_sum;
} config = {
  .correct_negative_alpha = true,
  .correct_sum = false,
};

static int compute_gradient(
    const size_t nitems,
    const double * const h,
    const double * const alpha,
    double * const gradient
) {
  for (size_t i = 0; i < nitems; i++) {
    gradient[i] = 1.;
    for (size_t j = 0; j < nitems; j++) {
      gradient[i] -= h[i * nitems + j] * alpha[j];
    }
  }
  return 0;
}

static int check_proper_classification(
    const size_t nitems,
    const dataset_t * const items,
    const vector_t * const weights
) {
  // confirm if all training datasets are classified properly
  for (size_t i = 0; i < nitems; i++) {
    const dataset_t * const item = items + i;
    const double predicted = vector_op.inner_product(weights, &item->vector);
    if (predicted * item->label < 0.) {
      printf(
          "failed to train: % .7e % .7e, predicted % .7e while it should be % .1f\n",
          item->vector[1],
          item->vector[2],
          predicted,
          item->label
      );
    }
  }
  return 0;
}

int train(
    const size_t nitems,
    dataset_t * const items,
    vector_t * const weights
) {
  double * const h = malloc(nitems * nitems * sizeof(double));
  for (size_t i = 0; i < nitems; i++) {
    const double yi = items[i].label;
    const vector_t * const vector_i = &items[i].vector;
    for (size_t j = 0; j < nitems; j++) {
      const double yj = items[j].label;
      const vector_t * const vector_j = &items[j].vector;
      h[i * nitems + j] = yi * yj * vector_op.inner_product(vector_i, vector_j);
    }
  }
  double * const alpha = malloc(nitems * sizeof(double));
  double * const gradient = malloc(nitems * sizeof(double));
  double * const new_alpha = malloc(nitems * sizeof(double));
  double * const new_gradient = malloc(nitems * sizeof(double));
  for (size_t i = 0; i < nitems; i++) {
    alpha[i] = 0.;
    gradient[i] = 0.;
  }
  const char log_file_name[] = DATA_DIRECTORY "training.log";
  FILE * const fp = fopen(log_file_name, "w");
  if (NULL == fp) {
    perror(log_file_name);
  }
  // gradient-descent
  const size_t iter_max = nitems << 8;
  double gamma = 1.e+0;
  for (size_t iter = 0; ; iter += 1) {
    // compute increment gradient to update alpha
    compute_gradient(nitems, h, alpha, gradient);
    double tolerance = 0.;
    while (1) {
      for (size_t i = 0; i < nitems; i++) {
        new_alpha[i] = alpha[i] + gamma * gradient[i];
      }
      // correct alpha to satisfy constraints
      if (config.correct_negative_alpha) {
        for (size_t i = 0; i < nitems; i++) {
          double * const a = new_alpha + i;
          if (*a < 0.) {
            *a = 0.;
          }
        }
      }
      if (config.correct_sum) {
        double sum = 0.;
        size_t n_non_zero_items = 0;
        for (size_t i = 0; i < nitems; i++) {
          if (new_alpha[i] < DBL_EPSILON) {
            continue;
          }
          sum += new_alpha[i] * items[i].label;
          n_non_zero_items += 1;
        }
        sum /= n_non_zero_items;
        for (size_t i = 0; i < nitems; i++) {
          if (new_alpha[i] < DBL_EPSILON) {
            continue;
          }
          new_alpha[i] -= sum * items[i].label;
        }
      }
      compute_gradient(nitems, h, new_alpha, new_gradient);
      double numerator = 0.;
      double denominator = 0.;
      for (size_t i = 0; i < nitems; i++) {
        const double dalpha = new_alpha[i] - alpha[i];
        const double dgradient = new_gradient[i] - gradient[i];
        numerator += dalpha * dgradient;
        denominator += dgradient * dgradient;
      }
      if (denominator < DBL_EPSILON) {
        goto terminate;
      }
      const double new_gamma = fabs(numerator) / denominator;
      if (gamma <= new_gamma) {
        tolerance = 0.;
        for (size_t i = 0; i < nitems; i++) {
          tolerance = fmax(tolerance, fabs(new_alpha[i] - alpha[i]));
          alpha[i] = new_alpha[i];
        }
        break;
      } else {
        gamma = new_gamma;
      }
    }
    // evaluate objective function
    double objective = 0.;
    double sum = 0.;
    for (size_t i = 0; i < nitems; i++) {
      objective += alpha[i];
      for (size_t j = 0; j < nitems; j++) {
        objective -= 0.5 * alpha[i] * alpha[j] * h[i * nitems + j];
      }
      sum += alpha[i] * items[i].label;
    }
    if (NULL != fp) {
      fprintf(fp, "%zu % .7e % .7e % .7e % .7e\n", iter, gamma, objective, tolerance, sum);
    }
    if (iter_max < iter) {
      break;
    }
  }
terminate:
  if (NULL != fp) {
    fclose(fp);
  }
  {
    FILE * const fp = fopen(DATA_DIRECTORY "trained_alpha.dat", "w");
    for (size_t i = 0; i < nitems; i++) {
      fprintf(fp, "%zu % .7e\n", i, alpha[i]);
    }
    fclose(fp);
  }
  (*weights)[0] = 0.;
  (*weights)[1] = 0.;
  (*weights)[2] = 0.;
  for (size_t i = 0; i < nitems; i++) {
    const dataset_t * const item = items + i;
    const vector_t * const vector = &item->vector;
    const double label = item->label;
    (*weights)[1] += alpha[i] * label * (*vector)[1];
    (*weights)[2] += alpha[i] * label * (*vector)[2];
  }
  size_t n_support_vectors = 0;
  for (size_t i = 0; i < nitems; i++) {
    if (alpha[i] < DBL_EPSILON) {
      continue;
    }
    dataset_t * const item = items + i;
    const vector_t * const vector = &item->vector;
    const double label = item->label;
    item->is_support_vector = true;
    (*weights)[0] += label;
    (*weights)[0] -= (*weights)[1] * (*vector)[1];
    (*weights)[0] -= (*weights)[2] * (*vector)[2];
    n_support_vectors += 1;
  }
  (*weights)[0] /= n_support_vectors;
  check_proper_classification(nitems, items, weights);
  free(alpha);
  free(gradient);
  free(new_alpha);
  free(new_gradient);
  free(h);
  return 0;
}

