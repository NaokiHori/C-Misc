#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include "./snpyio.h"

#define NDIMS 2

static const double PI = 3.14159265358979324;

static int compute_matrix_vector_product(
    const size_t nitems,
    const double * const matrix,
    const double * const vector,
    double * const result
) {
  for (size_t j = 0; j < nitems; j++) {
    double * const value = result + j;
    *value = 0.;
    for (size_t i = 0; i < nitems; i++) {
      *value += matrix[j * nitems + i] * vector[i];
    }
  }
  return 0;
}

// grid-streching indicator
// 0: extremely stretched
// 1: uniform
static int build_coordinate(
    const double length,
    const size_t nitems,
    const double grad,
    double ** const faces,
    double ** const centers
) {
  *faces = malloc((nitems + 1) * sizeof(double));
  *centers = malloc(nitems * sizeof(double));
  for (size_t i = 0; i < nitems + 1; i++) {
    const double x = 1. * i * length / nitems;
    (*faces)[i] =
      + (2. * grad - 2.) * (x * x * x)
      + (- 3. * grad + 3.) * (x * x)
      + grad * x;
  }
  for (size_t i = 0; i < nitems; i++) {
    (*centers)[i] = 0.5 * (*faces)[i] + 0.5 * (*faces)[i + 1];
  }
  return 0;
}

static int build_solver(
    const size_t nitems,
    const double * const lower_diagonal,
    const double * const main_diagonal,
    const double * const upper_diagonal,
    const double * const symmetrization_operator,
    double ** const eigenvalues,
    double ** const forward_operator,
    double ** const backward_operator
) {
  // build symmetric operator
  // TODO: using full NxN matrix is redundant as this is also tri-diagonal
  double * const symmetric_operator = malloc(nitems * nitems * sizeof(double));
  for (size_t j = 0; j < nitems; j++) {
    for (size_t i = 0; i < nitems; i++) {
      symmetric_operator[j * nitems + i] = 0.;
    }
  }
  for (size_t j = 0; j < nitems; j++) {
    if (0 < j) {
      const size_t i = j - 1;
      symmetric_operator[j * nitems + i] = symmetrization_operator[j] * lower_diagonal[j] / symmetrization_operator[i];
    }
    {
      const size_t i = j;
      symmetric_operator[j * nitems + i] = symmetrization_operator[j] * main_diagonal[j] / symmetrization_operator[i];
    }
    if (j < nitems - 1) {
      const size_t i = j + 1;
      symmetric_operator[j * nitems + i] = symmetrization_operator[j] * upper_diagonal[j] / symmetrization_operator[i];
    }
  }
  // assert symmetry
  {
    double error = 0.;
    for (size_t j = 0; j < nitems; j++) {
      for (size_t i = 0; i < nitems; i++) {
        const double sij = symmetric_operator[j * nitems + i];
        const double sji = symmetric_operator[i * nitems + j];
        error += pow(sij - sji, 2.);
      }
    }
    printf("symmetry error: % .1e\n", sqrt(error / nitems / nitems));
  }
  // TODO: use in-house eigenvalue algorithm
  *eigenvalues = malloc(nitems * sizeof(double));
  double * const eigenvectors = malloc(nitems * nitems * sizeof(double));
  {
    FILE * fp = NULL;
    fp = fopen("symmetric_operator.npy", "w");
    size_t header_size = 0;
    snpyio_w_header(2, (size_t []){nitems, nitems}, "'<f8'", false, fp, &header_size);
    fwrite(symmetric_operator, sizeof(double), nitems * nitems, fp);
    fclose(fp);
    system("python3 compute_eigenvalues.py");
    size_t ndims = 0;
    size_t * dims = NULL;
    char * dtype = NULL;
    bool is_fortran_order = false;
    fp = fopen("eigenvalues.npy", "r");
    snpyio_r_header(&ndims, &dims, &dtype, &is_fortran_order, fp, &header_size);
    free(dims);
    free(dtype);
    fread(*eigenvalues, sizeof(double), nitems, fp);
    fclose(fp);
    fp = fopen("eigenvectors.npy", "r");
    snpyio_r_header(&ndims, &dims, &dtype, &is_fortran_order, fp, &header_size);
    free(dims);
    free(dtype);
    fread(eigenvectors, sizeof(double), nitems * nitems, fp);
    fclose(fp);
  }
  // assert eigendecomposition
  {
    double * const a = malloc(nitems * nitems * sizeof(double));
    double * const b = malloc(nitems * nitems * sizeof(double));
    // revert eigendecomposition
    for (size_t j = 0; j < nitems; j++) {
      for (size_t i = 0; i < nitems; i++) {
        a[j * nitems + i] = eigenvectors[j * nitems + i] * (*eigenvalues)[i];
      }
    }
    for (size_t j = 0; j < nitems; j++) {
      for (size_t i = 0; i < nitems; i++) {
        double * const value = b + j * nitems + i;
        *value = 0.;
        for (size_t k = 0; k < nitems; k++) {
          *value += a[j * nitems + k] * eigenvectors[i * nitems + k];
        }
      }
    }
    double error = 0.;
    for (size_t j = 0; j < nitems; j++) {
      for (size_t i = 0; i < nitems; i++) {
        error += pow(symmetric_operator[j * nitems + i] - b[j * nitems + i], 2.);
      }
    }
    printf("eigendecomposition error: % .1e\n", sqrt(error / nitems / nitems));
    free(a);
    free(b);
  }
  // unified operators: qT x d and dINV x q
  *forward_operator = malloc(nitems * nitems * sizeof(double));
  *backward_operator = malloc(nitems * nitems * sizeof(double));
  for (size_t j = 0; j < nitems; j++) {
    for (size_t i = 0; i < nitems; i++) {
      (*forward_operator)[j * nitems + i] = eigenvectors[i * nitems + j] * symmetrization_operator[i];
    }
  }
  for (size_t j = 0; j < nitems; j++) {
    for (size_t i = 0; i < nitems; i++) {
      (*backward_operator)[j * nitems + i] = 1. / symmetrization_operator[j] * eigenvectors[j * nitems + i];
    }
  }
  free(symmetric_operator);
  free(eigenvectors);
  return 0;
}

int main(
    void
) {
  const double length = 1.;
  const size_t nitems = 23;
  // coordinates
  double * faces = NULL;
  double * centers = NULL;
  if (0 != build_coordinate(length, nitems, 0.25, &faces, &centers)) {
    return 1;
  }
  // laplace operator (tri-diagonal)
  double * const lower_diagonal = malloc(nitems * sizeof(double));
  double * const main_diagonal = malloc(nitems * sizeof(double));
  double * const upper_diagonal = malloc(nitems * sizeof(double));
  {
    const size_t n = 0;
    const double upper = 1. / (faces[n + 1] - faces[n]) / (centers[n + 1] - centers[n]);
    lower_diagonal[n] = nan(""); // not in use
    main_diagonal[n] = - upper;
    upper_diagonal[n] = upper;
  }
  for (size_t n = 1; n < nitems - 1; n++) {
    const double lower = 1. / (faces[n + 1] - faces[n]) / (centers[n] - centers[n - 1]);
    const double upper = 1. / (faces[n + 1] - faces[n]) / (centers[n + 1] - centers[n]);
    lower_diagonal[n] = lower;
    main_diagonal[n] = - lower - upper;
    upper_diagonal[n] = upper;
  }
  {
    const size_t n = nitems - 1;
    const double lower = 1. / (faces[n + 1] - faces[n]) / (centers[n] - centers[n - 1]);
    lower_diagonal[n] = lower;
    main_diagonal[n] = - lower;
    upper_diagonal[n] = nan(""); // not in use
  }
  // operator to symmetrize laplace operator
  // NOTE: a vector is used (not full NxN array) since it is diagonal
  double * const symmetrization_operator = malloc(nitems * sizeof(double));
  for (size_t n = 0; n < nitems; n++) {
    symmetrization_operator[n] = sqrt(faces[n + 1] - faces[n]);
  }
  // build solver
  double * eigenvalues = NULL;
  double * forward_operator = NULL;
  double * backward_operator = NULL;
  if (0 != build_solver(
      nitems,
      lower_diagonal,
      main_diagonal,
      upper_diagonal,
      symmetrization_operator,
      &eigenvalues,
      &forward_operator,
      &backward_operator
  )) {
    return 1;
  }
  // check eigenvalues
  {
    puts("---eigenvalues---");
    for (size_t i = 0; i < nitems; i++) {
      printf("%zu % .15e\n", i, eigenvalues[i]);
    }
    puts("-----------------");
  }
  // right-hand-side vector
  double * const values = malloc(nitems * sizeof(double));
  for (size_t i = 0; i < nitems; i++) {
    const double x = centers[i];
    values[i] =
      + 25. * PI * PI * cos(5. * PI * x)
      + 12. * x - 6.;
  }
  // solve
  // auxiliary buffers for brevity
  double * const buffers[2] = {
    malloc(nitems * sizeof(double)),
    malloc(nitems * sizeof(double)),
  };
  // forward transform
  if (0 != compute_matrix_vector_product(nitems, forward_operator, values, buffers[0])) {
    return 1;
  }
  // solve in spectral space
  {
    const double * const buf0 = buffers[0];
    double * const buf1 = buffers[1];
    for (size_t i = 0; i < nitems; i++) {
      const double eigenvalue = eigenvalues[i];
      buf1[i] = fabs(eigenvalue) < 1e-12 ? 0. : buf0[i] / eigenvalue;
    }
  }
  // backward transform
  if (0 != compute_matrix_vector_product(nitems, backward_operator, buffers[1], buffers[0])) {
    return 1;
  }
  // check residual
  {
    const double * const solution = buffers[0];
    double * const differentiated = malloc(nitems * sizeof(double));
    for (size_t n = 0; n < nitems; n++) {
      const double components[3] = {
        0 == n ? 0. : lower_diagonal[n] * solution[n - 1],
        main_diagonal[n] * solution[n],
        nitems - 1 == n ? 0. : upper_diagonal[n] * solution[n + 1],
      };
      differentiated[n] = components[0] + components[1] + components[2];
    }
    double error = 0.;
    for (size_t n = 0; n < nitems; n++) {
      error += pow(differentiated[n] - values[n], 2.);
    }
    printf("residual: % .1e\n", sqrt(error / nitems));
    free(differentiated);
  }
  // output solution
  {
    const double * const buf = buffers[0];
    FILE * const fp = fopen("answer.dat", "w");
    for (size_t i = 0; i < nitems; i++) {
      fprintf(fp, "% .15e % .15e\n", centers[i], buf[i]);
    }
    fclose(fp);
  }
  free(faces);
  free(centers);
  free(lower_diagonal);
  free(main_diagonal);
  free(upper_diagonal);
  free(symmetrization_operator);
  free(eigenvalues);
  free(forward_operator);
  free(backward_operator);
  free(values);
  free(buffers[0]);
  free(buffers[1]);
  return 0;
}

