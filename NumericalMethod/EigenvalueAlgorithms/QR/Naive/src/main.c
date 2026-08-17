#include <math.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

// apply Givens rotation from the left of matrix
static int rotate(
    const size_t nitems,
    const size_t target_i,
    const size_t target_j,
    double * const matrix,
    double * const orthogonal_matrix
) {
  // find rotation angle
  const double values[2] = {
    matrix[target_j * nitems + target_j],
    matrix[target_i * nitems + target_j],
  };
  const double r = hypot(values[0], values[1]);
  const double c = 0. == r ? 1. : + values[0] / r;
  const double s = 0. == r ? 0. : - values[1] / r;
  // apply rotations from the left to triangulate the matrix
  // NOTE: we can introduce j = j_start to optimize if all left columns are already eliminated
  //   (depending on the matrix and order)
  for (size_t j = 0; j < nitems; j++) {
    double * const elements[2] = {
      matrix + target_j * nitems + j,
      matrix + target_i * nitems + j,
    };
    const double values[2] = {
      *elements[0],
      *elements[1],
    };
    *elements[0] = target_j == j ? r  : c * values[0] - s * values[1];
    *elements[1] = target_j == j ? 0. : s * values[0] + c * values[1];
  }
  // apply transpose of rotation matrix from the right to update orthogonal matrix
  // NOTE: we can introduce i = i_end to optimize if all bottom rows have not been touched
  //   (depending on the matrix and order)
  for (size_t i = 0; i < nitems; i++) {
    double * const elements[2] = {
      orthogonal_matrix + i * nitems + target_i,
      orthogonal_matrix + i * nitems + target_j,
    };
    const double values[2] = {
      *elements[0],
      *elements[1],
    };
    *elements[0] = + c * values[0] + s * values[1];
    *elements[1] = - s * values[0] + c * values[1];
  }
  return 0;
}

static int compute_matrix_multiplication(
    const size_t nitems,
    const double * const a,
    const double * const b,
    double * const c
) {
  for (size_t i = 0; i < nitems; i++) {
    for (size_t j = 0; j < nitems; j++) {
      double * const result = c + i * nitems + j;
      *result = 0.;
      for (size_t k = 0; k < nitems; k++) {
        *result += a[i * nitems + k] * b[k * nitems + j];
      }
    }
  }
  return 0;
}

static int build_identity_matrix(
    const size_t nitems,
    double * const matrix
) {
  for (size_t i = 0; i < nitems; i++) {
    for (size_t j = 0; j < nitems; j++) {
      matrix[i * nitems + j] = i == j ? 1. : 0.;
    }
  }
  return 0;
}

static int compute_frobenius_norm(
    const size_t nitems,
    const double * const matrix,
    double * const norm
) {
  *norm = 0.;
  for (size_t n = 0; n < nitems * nitems; n++) {
    *norm += pow(matrix[n], 2.);
  }
  return 0;
}

static int compute_l2_off_diagonals(
    const size_t nitems,
    const double * const matrix,
    double * const l2
) {
  *l2 = 0.;
  for (size_t i = 0; i < nitems; i++) {
    for (size_t j = 0; j < nitems; j++) {
      if (i == j) {
        continue;
      }
      *l2 += pow(matrix[i * nitems + j], 2.);
    }
  }
  *l2 = sqrt(*l2 / nitems / (nitems - 1));
  return 0;
}

static int compare_double_values(
    const void * const a,
    const void * const b
) {
  const double double_a = *(double *)a;
  const double double_b = *(double *)b;
  if (double_a < double_b) {
    return -1;
  } else {
    return 1;
  }
}

static int check_eigendecomposition(
    const size_t nitems,
    const double * const original_matrix,
    double * const expected_eigenvalues,
    const double * const eigenvalues,
    const double * const eigenvectors
) {
  // assert eigenvalues by comparing with the reference result
  {
    // sort eigenvalues
    double * const sorted_eigenvalues = malloc(nitems * sizeof(double));
    for (size_t i = 0; i < nitems; i++) {
      sorted_eigenvalues[i] = eigenvalues[i];
    }
    qsort(sorted_eigenvalues, nitems, sizeof(double), compare_double_values);
    qsort(expected_eigenvalues, nitems, sizeof(double), compare_double_values);
    // compare
    double error = 0.;
    for (size_t i = 0; i < nitems; i++) {
      error += pow(sorted_eigenvalues[i] - expected_eigenvalues[i], 2.);
    }
    error = sqrt(error / nitems);
    printf("|| lambda - lambda^ref ||_2: % .1e\n", error);
    // clean-up
    free(sorted_eigenvalues);
  }
  // assert eigenvectors (orthogonality)
  {
    double error = 0.;
    for (size_t i = 0; i < nitems; i++) {
      for (size_t j = 0; j < nitems; j++) {
        double multiplied = 0.;
        for (size_t k = 0; k < nitems; k++) {
          multiplied += eigenvectors[k * nitems + i] * eigenvectors[k * nitems + j];
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
          result += original_matrix[i * nitems + k] * eigenvectors[k * nitems + j];
        }
        error += pow(result - eigenvectors[i * nitems + j] * eigenvalues[j], 2.);
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
#define NITEMS 3
  const double original_matrix[NITEMS * NITEMS] = {
    6., 5., 0.,
    5., 1., 4.,
    0., 4., 3.,
  };
  // ping-pong buffers
  double matrices[2][NITEMS * NITEMS] = {{0.}, {0.}};
  for (size_t n = 0; n < NITEMS * NITEMS; n++) {
    matrices[0][n] = original_matrix[n];
  }
  // reference result by numpy.linalg.eigh
  double expected_eigenvalues[NITEMS] = {
    -3.864921474506954e+00,
    +4.021759941158753e+00,
    +9.843161533348200e+00,
  };
  double initial_frobenius_norm = 0.;
  compute_frobenius_norm(NITEMS, matrices[0], &initial_frobenius_norm);
  // qr algorithm
  // A^{k + 1} = R^k Q^k = QT^k A^k Q^k
  double eigenvectors[2][NITEMS * NITEMS] = {{0.}, {0.}};
  build_identity_matrix(NITEMS, eigenvectors[0]);
  for (size_t iter = 0; iter < (1 << 10); iter++) {
    double q[NITEMS * NITEMS] = {0.};
    build_identity_matrix(NITEMS, q);
    double * const input = 0 == iter % 2 ? matrices[0] : matrices[1];
    double * const output = 0 == iter % 2 ? matrices[1] : matrices[0];
    // eliminate lower triangle
    for (size_t i = 0; i < NITEMS; i++) {
      for (size_t j = 0; j < i; j++) {
        rotate(NITEMS, i, j, input, q);
      }
    }
    // compute next-step matrix
    compute_matrix_multiplication(NITEMS, input, q, output);
    // compute eigenvectors
    double * const eigenvectors_current = 0 == iter % 2 ? eigenvectors[0] : eigenvectors[1];
    double * const eigenvectors_next = 0 == iter % 2 ? eigenvectors[1] : eigenvectors[0];
    compute_matrix_multiplication(NITEMS, eigenvectors_current, q, eigenvectors_next);
    // monitor
    double frobenius_norm = 0.;
    compute_frobenius_norm(NITEMS, output, &frobenius_norm);
    double l2_off_diagonals = 0.;
    compute_l2_off_diagonals(NITEMS, output, &l2_off_diagonals);
    printf(
        "%zu % .1e % .1e\n",
        iter,
        (frobenius_norm - initial_frobenius_norm) / initial_frobenius_norm,
        l2_off_diagonals
    );
  }
  double eigenvalues[NITEMS] = {0.};
  for (size_t n = 0; n < NITEMS; n++) {
    eigenvalues[n] = matrices[0][n * NITEMS + n];
  }
  check_eigendecomposition(
      NITEMS,
      original_matrix,
      expected_eigenvalues,
      eigenvalues,
      eigenvectors[0]
  );
  return 0;
}

