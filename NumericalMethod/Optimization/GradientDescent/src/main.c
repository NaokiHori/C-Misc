#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <stdbool.h>
#include <math.h>
#include "./snpyio.h"

#define NDIMS 2

typedef double vector_t[NDIMS];

static const double pi = 3.141592653589793;

static double f(
    const size_t nfreqs,
    const double * const amplitudes,
    const double * const shifts,
    const double x
) {
  double result = 0.;
  for (size_t n = 0; n < nfreqs; n++) {
    result += amplitudes[n] * sin(1. * n * x + shifts[n]);
  }
  return result;
}

static double dfdx(
    const size_t nfreqs,
    const double * const amplitudes,
    const double * const shifts,
    const double x
) {
  double result = 0.;
  for (size_t n = 0; n < nfreqs; n++) {
    result += 1. * n * amplitudes[n] * cos(1. * n * x + shifts[n]);
  }
  return result;
}

static double compute_gradient(
    const size_t dir,
    const size_t nfreqs,
    double * const * const amplitudes,
    double * const * const shifts,
    const vector_t * const vector
) {
  double result = 1.;
  for (size_t dim = 0; dim < NDIMS; dim++) {
    if (dir == dim) {
      result *= dfdx(nfreqs, amplitudes[dim], shifts[dim], (*vector)[dim]);
    } else {
      result *= f(nfreqs, amplitudes[dim], shifts[dim], (*vector)[dim]);
    }
  }
  return result;
}

static double norm(
    const vector_t * const vector
) {
  double result = 0.;
  for (size_t dim = 0; dim < NDIMS; dim++) {
    result += pow((*vector)[dim], 2.);
  }
  return sqrt(result);
}

int main(
    void
) {
  srand(2);
  const double lengths[NDIMS] = {2. * pi, 2. * pi};
  const size_t nfreqs = 8;
  double * const amplitudes[NDIMS] = {
    malloc(nfreqs * sizeof(double)),
    malloc(nfreqs * sizeof(double)),
  };
  double * const shifts[NDIMS] = {
    malloc(nfreqs * sizeof(double)),
    malloc(nfreqs * sizeof(double)),
  };
  for (size_t dim = 0; dim < NDIMS; dim++) {
    for (size_t n = 0; n < nfreqs; n++) {
      amplitudes[dim][n] = 1. / pow(1. * nfreqs, 5. / 3.) * rand() / RAND_MAX;
      shifts[dim][n] = 2. * pi * rand() / RAND_MAX;
    }
  }
  if (NDIMS == 2) {
    const size_t nx = 128;
    const size_t ny = 128;
    double * const potential = malloc(nx * ny * sizeof(double));
    for (size_t i = 0; i < ny; i++) {
      const double y = (i + 0.5) * lengths[1] / ny;
      for (size_t j = 0; j < nx; j++) {
        const double x = (j + 0.5) * lengths[0] / nx;
        potential[i * nx + j] = f(nfreqs, amplitudes[0], shifts[0], x) * f(nfreqs, amplitudes[1], shifts[1], y);
      }
    }
    FILE * const fp = fopen("potential.npy", "w");
    size_t header_size = 0;
    snpyio_w_header(2, (size_t [2]){ny, nx}, "'<f8'", false, fp, &header_size);
    fwrite(potential, sizeof(double), nx * ny, fp);
    fclose(fp);
    free(potential);
  }
  FILE * const fp = fopen("vectors.dat", "w");
  const size_t nvectors = 128;
  for (size_t n = 0; n < nvectors; n++) {
    vector_t vector = {0};
    for (size_t dim = 0; dim < NDIMS; dim++) {
      vector[dim] = lengths[dim] * rand() / RAND_MAX;
    }
    const size_t iter_max = 1 << 10;
    double gamma = 1e+0;
    for (size_t iter = 0; ; iter++) {
      vector_t gradient = {0};
      for (size_t dim = 0; dim < NDIMS; dim++) {
        gradient[dim] = compute_gradient(dim, nfreqs, amplitudes, shifts, &vector);
      }
      while (1) {
        vector_t new_vector = {0};
        for (size_t dim = 0; dim < NDIMS; dim++) {
          new_vector[dim] = vector[dim] - gamma * gradient[dim];
        }
        vector_t new_gradient = {0};
        for (size_t dim = 0; dim < NDIMS; dim++) {
          new_gradient[dim] = compute_gradient(dim, nfreqs, amplitudes, shifts, &new_vector);
        }
        double numerator = 0.;
        double denominator = 0.;
        for (size_t dim = 0; dim < NDIMS; dim++) {
          const double dvector = new_vector[dim] - vector[dim];
          const double dgradient = new_gradient[dim] - gradient[dim];
          numerator += dvector * dgradient;
          denominator += dgradient * dgradient;
        }
        const double gamma_max = fabs(numerator) / denominator;
        if (gamma <= gamma_max) {
          for (size_t dim = 0; dim < NDIMS; dim++) {
            vector[dim] = new_vector[dim];
          }
          gamma = gamma_max;
          break;
        } else {
          gamma = gamma_max;
        }
      }
      const double change = norm(&gradient);
      fprintf(fp, "%zu % .7e % .7e % .7e % .7e\n", iter, vector[0], vector[1], change, gamma);
      if (change < 1e-8) {
        break;
      }
      if (iter_max < iter) {
        printf("vector no. %zu, failed to converge: % .1e\n", n, change);
        break;
      }
    }
    fprintf(fp, "\n");
  }
  fclose(fp);
  for (size_t dim = 0; dim < NDIMS; dim++) {
    free(shifts[dim]);
  }
  return 0;
}
