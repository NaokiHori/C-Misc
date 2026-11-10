#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include "./gaussian_quadrature.h"

static int compute_matrix_vector_multiplication(
    const size_t nitems,
    const double * const matrix,
    const double * const inputs,
    double * const outputs
) {
  for (size_t i = 0; i < nitems; i++) {
    double * const output = outputs + i;
    *output = 0.;
    for (size_t j = 0; j < nitems; j++) {
      *output += matrix[i * nitems + j] * inputs[j];
    }
  }
  return 0;
}

static int check_backward(
    const size_t nitems,
    const double * const backward,
    const double * const roots
) {
  double * const spectral_values = malloc(nitems * sizeof(double));
  double * const physical_values = malloc(nitems * sizeof(double));
  // initialize spectral values
  for (size_t i = 0; i < nitems; i++) {
    spectral_values[i] = 2 == i ? 1. : 0.;
  }
  // transform from spectral to physical
  compute_matrix_vector_multiplication(nitems, backward, spectral_values, physical_values);
  double errors[2] = {0., 0.};
  for (size_t i = 0; i < nitems; i++) {
    const double root = roots[i];
    const double expected = 0.5 * (3. * pow(root, 2.) - 1.);
    const double obtained = physical_values[i];
    const double error = obtained - expected;
    errors[0] += pow(error, 2.);
    errors[1] = fmax(errors[1], fabs(error));
  }
  errors[0] = sqrt(errors[0] / nitems);
  printf("% .1e % .1e\n", errors[0], errors[1]);
  free(spectral_values);
  free(physical_values);
  return 0;
}

static int check_forward(
    const size_t nitems,
    const double * const forward,
    const double * const roots
) {
  double * const spectral_values = malloc(nitems * sizeof(double));
  double * const physical_values = malloc(nitems * sizeof(double));
  // initialize physical values
  for (size_t i = 0; i < nitems; i++) {
    physical_values[i] = 0.5 * (3. * pow(roots[i], 2.) - 1.);
  }
  // transform from physical to spectral
  compute_matrix_vector_multiplication(nitems, forward, physical_values, spectral_values);
  double errors[2] = {0., 0.};
  for (size_t k = 0; k < nitems; k++) {
    const double expected = 2 == k ? 1. : 0.;
    const double obtained = spectral_values[k];
    const double error = obtained - expected;
    errors[0] += pow(error, 2.);
    errors[1] = fmax(errors[1], fabs(error));
  }
  errors[0] = sqrt(errors[0] / nitems);
  printf("% .1e % .1e\n", errors[0], errors[1]);
  free(spectral_values);
  free(physical_values);
  return 0;
}

static int check_round_trip(
    const size_t nitems,
    const double * const forward,
    const double * const backward
) {
  double * const buffers[3] = {
    malloc(nitems * sizeof(double)),
    malloc(nitems * sizeof(double)),
    malloc(nitems * sizeof(double)),
  };
  double * const input = buffers[0];
  double * const expected = buffers[1];
  double * const tmp = buffers[2];
  double * const result = buffers[0];
  for (size_t i = 0; i < nitems; i++) {
    const double value = - 0.5 + 1. * rand() / RAND_MAX;
    input[i] = value;
    expected[i] = value;
  }
  compute_matrix_vector_multiplication(nitems, forward, input, tmp);
  compute_matrix_vector_multiplication(nitems, backward, tmp, result);
  double errors[2] = {0., 0.};
  for (size_t i = 0; i < nitems; i++) {
    const double error = result[i] - expected[i];
    errors[0] += pow(error, 2.);
    errors[1] = fmax(errors[1], fabs(error));
  }
  errors[0] = sqrt(errors[0] / nitems);
  printf("% .1e % .1e\n", errors[0], errors[1]);
  free(buffers[0]);
  free(buffers[1]);
  free(buffers[2]);
  return 0;
}

static int check_derivative(
    const size_t nitems,
    const double * const derivative,
    const double * const roots
) {
  double * const spectral_values = malloc(nitems * sizeof(double));
  double * const physical_values = malloc(nitems * sizeof(double));
  for (size_t i = 0; i < nitems; i++) {
    spectral_values[i] = 3 == i ? 1. : 0.;
  }
  compute_matrix_vector_multiplication(nitems, derivative, spectral_values, physical_values);
  double errors[2] = {0., 0.};
  for (size_t i = 0; i < nitems; i++) {
    const double root = roots[i];
    const double expected = 7.5 * root * root - 1.5;
    const double obtained = physical_values[i];
    const double error = obtained - expected;
    errors[0] += pow(error, 2.);
    errors[1] = fmax(errors[1], fabs(error));
  }
  errors[0] = sqrt(errors[0] / nitems);
  printf("% .1e % .1e\n", errors[0], errors[1]);
  free(spectral_values);
  free(physical_values);
  return 0;
}

static int check_derivative_2(
    const size_t nitems,
    const double * const derivative_2,
    const double * const roots
) {
  double * const spectral_values = malloc(nitems * sizeof(double));
  double * const physical_values = malloc(nitems * sizeof(double));
  for (size_t i = 0; i < nitems; i++) {
    spectral_values[i] = 3 == i ? 1. : 0.;
  }
  compute_matrix_vector_multiplication(nitems, derivative_2, spectral_values, physical_values);
  double errors[2] = {0., 0.};
  for (size_t i = 0; i < nitems; i++) {
    const double root = roots[i];
    const double expected = 15. * root;
    const double obtained = physical_values[i];
    const double error = obtained - expected;
    errors[0] += pow(error, 2.);
    errors[1] = fmax(errors[1], fabs(error));
  }
  errors[0] = sqrt(errors[0] / nitems);
  printf("% .1e % .1e\n", errors[0], errors[1]);
  free(spectral_values);
  free(physical_values);
  return 0;
}

int main(
    void
) {
  const size_t nitems = 128;
  double * const roots = malloc(nitems * sizeof(double));
  double * const weights = malloc(nitems * sizeof(double));
  if (0 != find_gaussian_quadrature(nitems, roots, weights)) {
    return 1;
  }
  double * const forward = malloc(nitems * nitems * sizeof(double));
  double * const backward = malloc(nitems * nitems * sizeof(double));
  double * const derivative = malloc(nitems * nitems * sizeof(double));
  double * const derivative_2 = malloc(nitems * nitems * sizeof(double));
  // build transform operators
  // i: root, k: frequency
  for (size_t i = 0; i < nitems; i++) {
    const double root = roots[i];
    backward[i * nitems + 0] = 1.;
    backward[i * nitems + 1] = root;
    for (size_t k = 2; k < nitems; k++) {
      backward[i * nitems + k] =
        + (2. * k - 1.) / k * root * backward[i * nitems + k - 1]
        - (k - 1.) / k * backward[i * nitems + k - 2];
    }
  }
  for (size_t k = 0; k < nitems; k++) {
    for (size_t i = 0; i < nitems; i++) {
      forward[k * nitems + i] = 0.5 * (2. * k + 1.) * weights[i] * backward[i * nitems + k];
    }
  }
  for (size_t i = 0; i < nitems; i++) {
    const double root = roots[i];
    derivative[i * nitems + 0] = 0.;
    for (size_t k = 1; k < nitems; k++) {
      derivative[i * nitems + k] =
        + k * backward[i * nitems + k - 1]
        + root * derivative[i * nitems + k - 1];
    }
  }
  for (size_t i = 0; i < nitems; i++) {
    const double root = roots[i];
    derivative_2[i * nitems + 0] = 0.;
    for (size_t k = 1; k < nitems; k++) {
      derivative_2[i * nitems + k] =
        + (k + 1.) * derivative[i * nitems + k - 1]
        + root * derivative_2[i * nitems + k - 1];
    }
  }
  check_backward(nitems, backward, roots);
  check_forward(nitems, forward, roots);
  check_round_trip(nitems, forward, backward);
  check_derivative(nitems, derivative, roots);
  check_derivative_2(nitems, derivative_2, roots);
  free(forward);
  free(backward);
  free(derivative);
  free(derivative_2);
  free(roots);
  free(weights);
  return 0;
}

