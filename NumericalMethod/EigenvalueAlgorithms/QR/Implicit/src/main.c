#include <math.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include "eigendecomposition.h"

static int compare_double_values(
    const void * const a,
    const void * const b
) {
  const double * const double_a = a;
  const double * const double_b = b;
  if (*double_a < *double_b) {
    return -1;
  } else {
    return 1;
  }
}

static int compare_eigen_pairs(
    const void * const a,
    const void * const b
) {
  const eigen_pair_t * const eigen_pair_a = a;
  const eigen_pair_t * const eigen_pair_b = b;
  if (eigen_pair_a->eigenvalue < eigen_pair_b->eigenvalue) {
    return -1;
  } else {
    return 1;
  }
}

static int build_tridiagonal_matrix(
    const size_t nitems,
    const double main_diagonal,
    const double sub_diagonal,
    double * const main_diagonals,
    double * const sub_diagonals,
    double * const expected_eigenvalues
) {
  for (size_t n = 0; n < nitems; n++) {
    main_diagonals[n] = main_diagonal;
  }
  for (size_t n = 0; n < nitems - 1; n++) {
    sub_diagonals[n] = sub_diagonal;
  }
  // eigenvalues of Tridiagonal-Toeplitz matrix can be computed analytically
  // https://de.wikipedia.org/wiki/Tridiagonal-Toeplitz-Matrix
  for (size_t i = 0; i < nitems; i++) {
    const double pi = 3.14159265358979324;
    expected_eigenvalues[i] = main_diagonal - 2. * sub_diagonal * cos(pi * (i + 1) / (nitems + 1));
  }
  qsort(expected_eigenvalues, nitems, sizeof(double), compare_double_values);
  return 0;
}

static int check_eigendecomposition(
    const size_t nitems,
    const double * const original_main_diagonals,
    const double * const original_sub_diagonals,
    double * const expected_eigenvalues,
    const double * const eigenvalues,
    const double * const eigenvectors_transposed
) {
  // assert eigenvalues by comparing with the reference result
  {
    // compare
    double error = 0.;
    for (size_t i = 0; i < nitems; i++) {
      error += pow(eigenvalues[i] - expected_eigenvalues[i], 2.);
    }
    error = sqrt(error / nitems);
    printf("|| lambda - lambda^ref ||_2: % .1e\n", error);
  }
  // assert eigenvectors (orthogonality)
  {
    double error = 0.;
    for (size_t i = 0; i < nitems; i++) {
      for (size_t j = 0; j < nitems; j++) {
        double multiplied = 0.;
        for (size_t k = 0; k < nitems; k++) {
          multiplied +=
            eigenvectors_transposed[i * nitems + k]
            *
            eigenvectors_transposed[j * nitems + k];
        }
        error += pow(multiplied - (i == j ? 1. : 0.), 2.);
      }
    }
    error = sqrt(error / nitems / nitems);
    printf("|| I - V^T @ V ||_F: % .1e\n", error);
  }
  // assert definition
  {
    double error = 0.;
    for (size_t i = 0; i < nitems; i++) {
      for (size_t j = 0; j < nitems; j++) {
        double result = 0.;
        for (size_t k = 0; k < nitems; k++) {
          const double matrix_component = i == k ? original_main_diagonals[i]
            : i + 1 == k ? original_sub_diagonals[i]
            : k + 1 == i ? original_sub_diagonals[k]
            : 0.;
          result += matrix_component * eigenvectors_transposed[j * nitems + k];
        }
        error += pow(result - eigenvectors_transposed[j * nitems + i] * eigenvalues[j], 2.);
      }
    }
    error = sqrt(error / nitems / nitems);
    printf("|| A @ V - V @ diag(lambda) ||_F: % .1e\n", error);
  }
  return 0;
}

int main(
    void
) {
#define NITEMS 128
  double original_main_diagonals[NITEMS] = {0.};
  double original_sub_diagonals[NITEMS - 1] = {0.};
  double main_diagonals[NITEMS] = {0.};
  double sub_diagonals[NITEMS - 1] = {0.};
  double expected_eigenvalues[NITEMS] = {0.};
  build_tridiagonal_matrix(
      NITEMS,
      -2.,
      1.,
      original_main_diagonals,
      original_sub_diagonals,
      expected_eigenvalues
  );
  for (size_t n = 0; n < NITEMS; n++) {
    main_diagonals[n] = original_main_diagonals[n];
  }
  for (size_t n = 0; n < NITEMS - 1; n++) {
    sub_diagonals[n] = original_sub_diagonals[n];
  }
  // qr algorithm
  // A^{k + 1} = R^k Q^k = QT^k A^k Q^k
  double eigenvectors_transposed[NITEMS * NITEMS] = {0.};
  compute_eigendecomposition(
      NITEMS,
      main_diagonals,
      sub_diagonals,
      eigenvectors_transposed
  );
  sort_eigendecomposition(
      NITEMS,
      compare_eigen_pairs,
      main_diagonals,
      eigenvectors_transposed
  );
  check_eigendecomposition(
      NITEMS,
      original_main_diagonals,
      original_sub_diagonals,
      expected_eigenvalues,
      main_diagonals,
      eigenvectors_transposed
  );
  return 0;
}

