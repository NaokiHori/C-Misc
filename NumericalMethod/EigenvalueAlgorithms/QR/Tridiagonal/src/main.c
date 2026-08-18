#include <math.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

static inline size_t min_size_t(
    const size_t a,
    const size_t b
) {
  return a < b ? a : b;
}

// apply Givens rotation from the left of tridiagonal matrix
static int rotate_tridiagonal(
    const size_t nitems,
    const size_t target_i,
    double * const matrix,
    double * const orthogonal_matrix
) {
  if (0 == target_i) {
    return 1;
  }
  // by assuming tridiagonal input, Givens rotations are applied only to subdiagonal components,
  //   implying the following target_i / target_j relation
  const size_t target_j = target_i - 1;
  // find rotation angle
  const double values[2] = {
    matrix[target_j * nitems + target_j],
    matrix[target_i * nitems + target_j],
  };
  const double r = hypot(values[0], values[1]);
  const double c = 0. == r ? 1. : + values[0] / r;
  const double s = 0. == r ? 0. : - values[1] / r;
  // apply rotations from the left to triangulate the matrix
  // NOTE: since the matrix is tridiagonal,
  // - target_j + 2 < j is zero
  // - j < target_j is already zero (assuming the matrix is swept from the top)
  for (size_t j = target_j; j < min_size_t(nitems, target_j + 3); j++) {
    double * const elements[2] = {
      matrix + target_j * nitems + j,
      matrix + target_i * nitems + j,
    };
    const double values[2] = {
      *elements[0],
      *elements[1],
    };
    *elements[0] = target_j == j ? r  : + c * values[0] - s * values[1];
    *elements[1] = target_j == j ? 0. : + s * values[0] + c * values[1];
  }
  // apply transpose of rotation matrix from the right to update orthogonal matrix
  // NOTE: since target_i < i-th rows have not been modified, we do not need to update
  for (size_t i = 0; i < target_i + 1; i++) {
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

static int multiply_r_and_q(
    const size_t nitems,
    const double * const r,
    const double * const q,
    double * const a
) {
  for (size_t i = 0; i < nitems; i++) {
    for (size_t j = 0; j < nitems; j++) {
      double * const result = a + i * nitems + j;
      *result = 0.;
      const size_t k_min = i;
      const size_t k_max = min_size_t(k_min + 3, nitems);
      for (size_t k = k_min; k < k_max; k++) {
        *result += r[i * nitems + k] * q[k * nitems + j];
      }
    }
  }
  return 0;
}

static int compute_l2_off_tridiagonals(
    const size_t nitems,
    const double * const matrix,
    double * const l2
) {
  *l2 = 0.;
  for (size_t i = 0; i < nitems; i++) {
    for (size_t j = 0; j < nitems; j++) {
      if (i == j || i + 1 == j || j + 1 == i) {
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

static int build_tridiagonal_matrix(
    const size_t nitems,
    const double main_diagonal,
    const double sub_diagonal,
    double * const matrix,
    double * const expected_eigenvalues
) {
  for (size_t i = 0; i < nitems; i++) {
    for (size_t j = 0; j < nitems; j++) {
      double * const element = matrix + i * nitems + j;
      if (i == j) {
        *element = main_diagonal;
      } else if (i + 1 == j || j + 1 == i) {
        *element = sub_diagonal;
      } else {
        *element = 0.;
      }
    }
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

static int compute_l2_eigenvalue_difference(
    const size_t nitems,
    const double * const matrix,
    const double * const expected_eigenvalues,
    double * const difference
) {
  double * const eigenvalues = malloc(nitems * sizeof(double));
  for (size_t n = 0; n < nitems; n++) {
    eigenvalues[n] = matrix[n * nitems + n];
  }
  qsort(eigenvalues, nitems, sizeof(double), compare_double_values);
  *difference = 0.;
  for (size_t n = 0; n < nitems; n++) {
    *difference += pow(eigenvalues[n] - expected_eigenvalues[n], 2.);
  }
  free(eigenvalues);
  return 0;
}

int main(
    void
) {
#define NITEMS 8
  double matrices[2][NITEMS * NITEMS] = {{0.}, {0.}};
  double expected_eigenvalues[NITEMS] = {0.};
  build_tridiagonal_matrix(NITEMS, -2., 1., matrices[0], expected_eigenvalues);
  // qr algorithm
  // A^{k + 1} = R^k Q^k = QT^k A^k Q^k
  for (size_t iter = 0; iter < (1 << 7); iter++) {
    double q[NITEMS * NITEMS] = {0.};
    for (size_t i = 0; i < NITEMS; i++) {
      for (size_t j = 0; j < NITEMS; j++) {
        q[i * NITEMS + j] = i == j ? 1. : 0.;
      }
    }
    double * const input = 0 == iter % 2 ? matrices[0] : matrices[1];
    double * const output = 0 == iter % 2 ? matrices[1] : matrices[0];
    // eliminate lower triangle
    for (size_t i = 1; i < NITEMS; i++) {
      rotate_tridiagonal(NITEMS, i, input, q);
    }
    multiply_r_and_q(NITEMS, input, q, output);
    double l2_off_tridiagonals = 0.;
    compute_l2_off_tridiagonals(NITEMS, output, &l2_off_tridiagonals);
    double l2_eigenvalue_difference = 0.;
    compute_l2_eigenvalue_difference(
        NITEMS,
        output,
        expected_eigenvalues,
        &l2_eigenvalue_difference
    );
    printf(
        "%zu % .1e % .1e\n",
        iter,
        l2_off_tridiagonals,
        l2_eigenvalue_difference
    );
  }
  return 0;
}

