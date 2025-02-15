#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <errno.h>
#include <math.h>
#include "./solve_linear_system.h"

#define NDIMS 4

static const double X_MIN = -1.;
static const double X_MAX = 1.;

typedef double point_t[NDIMS];

double f(
    const double x
) {
  // a sample polynomial to create a dataset
  const double coefs[NDIMS] = {3. / 20., -47. / 80., - 3. / 10., 1.};
  double result = 0.;
  for (size_t dim = 0; dim < NDIMS; dim++) {
    result += coefs[dim] * pow(x, dim);
  }
  return result;
}

static int output_points(
    const char file_name[],
    const size_t npoints,
    const point_t * const points
) {
  errno = 0;
  FILE * const fp = fopen(file_name, "w");
  if (NULL == fp) {
    perror(file_name);
    return 1;
  }
  for (size_t n = 0; n < npoints; n++) {
    fprintf(fp, "%zu % .15e % .15e\n", n, points[n][0], points[n][1]);
  }
  fclose(fp);
  return 0;
}

static int output_polynomial(
    const char file_name[],
    const double * const coefs
) {
  errno = 0;
  FILE * const fp = fopen(file_name, "w");
  if (NULL == fp) {
    perror(file_name);
    return 1;
  }
  const size_t nsamples = 64;
  for (size_t n = 0; n < nsamples; n++) {
    const double x = (X_MAX - X_MIN) * (1. * n) / (nsamples - 1) + X_MIN;
    double y = 0.;
    for (size_t dim = 0; dim < NDIMS; dim++) {
      y += coefs[dim] * pow(x, dim);
    }
    fprintf(fp, "% .15e % .15e\n", x, y);
  }
  fclose(fp);
  return 0;
}

int main(
    void
) {
  const size_t npoints = 128;
  point_t * const points = malloc(npoints * sizeof(point_t));
  for (size_t n = 0; n < npoints; n++) {
    const double x = (X_MAX - X_MIN) * (1. * rand() / RAND_MAX) + X_MIN;
    // perturb data
    const double amp = 0.1;
    const double y = f(x) + 2. * amp * (1. * rand() / RAND_MAX - 0.5);
    points[n][0] = x;
    points[n][1] = y;
  }
  output_points("dataset.dat", npoints, points);
  // construct a linear system a x = b
  double * const a = malloc(NDIMS * NDIMS * sizeof(double));
  double * const b = malloc(NDIMS * sizeof(double));
  for (size_t i = 0; i < NDIMS; i++) {
    for (size_t j = 0; j < NDIMS; j++) {
      a[i * NDIMS + j] = 0.;
      for (size_t k = 0; k < npoints; k++) {
        const double x = points[k][0];
        a[i * NDIMS + j] += pow(x, i + j);
      }
    }
  }
  for (size_t i = 0; i < NDIMS; i++) {
    b[i] = 0.;
    for (size_t k = 0; k < npoints; k++) {
      const double x = points[k][0];
      const double y = points[k][1];
      b[i] += pow(x, i) * y;
    }
  }
  solve_linear_system(NDIMS, a, b);
  // output the polynomial
  output_polynomial("result.dat", b);
  free(points);
  free(a);
  free(b);
  return 0;
}

