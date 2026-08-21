#include <math.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

// compute Wilkinson's shift
static int compute_shift(
    const size_t nitems,
    const double * const matrix,
    const size_t n_end,
    double * const shift
) {
  if (n_end < 2) {
    return 1;
  }
  const double diagonals[2] = {
    matrix[(n_end - 2) * nitems + (n_end - 2)],
    matrix[(n_end - 1) * nitems + (n_end - 1)],
  };
  const double sub_diagonal = matrix[(n_end - 1) * nitems + (n_end - 2)];
  // solve characteristic equation and pick-up the one closer to diagonals[1]
  // the solutions are:
  //   0.5 * (diagonals[0] + diagonals[1])
  //   +-
  //   sqrt(sub_diagonal^2 + (0.5 * (diagonals[0] - diagonals[1]))^2)
  // solutions - diagonals[1] =
  //   D +- sqrt(sub_diagonal^2 + D^2),
  //   with D = 0.5 * (diagonals[0] - diagonals[1])
  // and thus the objective value is
  // solution =
  //   diagonals[1] + D - sign(D) * sqrt(sub_diagonal^2 + D^2)
  // since sub_diagonal tends to be small in magnitude (we aim to eliminate it in the QR iteration),
  //   we compute in a numerically stable form instead, by rationalizing it:
  // solution =
  //   diagonals[1] - sub_diagonal^2 / (D + sign(D) * sqrt(sub_diagonal^2 + D^2))
  const double half_difference = 0.5 * (diagonals[0] - diagonals[1]);
  const double sign_half_difference = half_difference < 0. ? -1. : 1.;
  *shift =
    diagonals[1]
    -
    pow(sub_diagonal, 2.) / (
        half_difference
        +
        sign_half_difference * hypot(sub_diagonal, half_difference)
    );
  return 0;
}

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
  // by assuming tridiagonal input, Givens rotations are applied only to sub-diagonal components,
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

static int compute_l2_sub_diagonals(
    const size_t nitems,
    const double * const matrix,
    double * const difference
) {
  if (nitems < 2) {
    return 1;
  }
  *difference = 0.;
  for (size_t n = 1; n < nitems; n++) {
    *difference += pow(matrix[n * nitems + (n - 1)], 2.);
  }
  *difference = sqrt(*difference / (nitems - 1));
  return 0;
}

int main(
    void
) {
#define NITEMS 32
  double matrices[2][NITEMS * NITEMS] = {{0.}, {0.}};
  double expected_eigenvalues[NITEMS] = {0.};
  build_tridiagonal_matrix(NITEMS, -2., 1., matrices[0], expected_eigenvalues);
  // qr algorithm
  // A^{k + 1} = R^k Q^k = QT^k A^k Q^k
  size_t n_end = NITEMS;
  // for safety: should not reach
  const size_t iter_max = 1024;
  for (size_t iter = 0;;) {
    double q[NITEMS * NITEMS] = {0.};
    for (size_t i = 0; i < NITEMS; i++) {
      for (size_t j = 0; j < NITEMS; j++) {
        q[i * NITEMS + j] = i == j ? 1. : 0.;
      }
    }
    double * const input = 0 == iter % 2 ? matrices[0] : matrices[1];
    double * const output = 0 == iter % 2 ? matrices[1] : matrices[0];
    // compute shift
    double shift = 0.;
    compute_shift(NITEMS, input, n_end, &shift);
    // apply shift
    for (size_t i = 0; i < n_end; i++) {
      input[i * NITEMS + i] -= shift;
    }
    // eliminate lower triangle
    for (size_t i = 1; i < n_end; i++) {
      rotate_tridiagonal(NITEMS, i, input, q);
    }
    // compute next matrix
    multiply_r_and_q(NITEMS, input, q, output);
    // detatch shift
    for (size_t i = 0; i < n_end; i++) {
      output[i * NITEMS + i] += shift;
    }
    iter += 1;
    //
    double l2_sub_diagonals = 0.;
    compute_l2_sub_diagonals(
        NITEMS,
        output,
        &l2_sub_diagonals
    );
    const double target_sub_diagonal = output[(n_end - 1) * NITEMS + (n_end - 2)];
    printf(
        "%zu % .1e % .1e\n",
        iter,
        l2_sub_diagonals,
        target_sub_diagonal
    );
    if (fabs(target_sub_diagonal) < 1e-15) {
      n_end -= 1;
    }
    if (n_end < 2) {
      // compare eigenvalues
      double eigenvalues[NITEMS] = {0.};
      for (size_t n = 0; n < NITEMS; n++) {
        eigenvalues[n] = output[n * NITEMS + n];
      }
      qsort(eigenvalues, NITEMS, sizeof(double), compare_double_values);
      double error = 0.;
      for (size_t n = 0; n < NITEMS; n++) {
        error += pow(eigenvalues[n] - expected_eigenvalues[n], 2.);
      }
      printf("eigenvalue error: % .1e\n", sqrt(error / NITEMS));
      break;
    }
    if (iter_max < iter) {
      printf("not converged after %zu iterations\n", iter);
      break;
    }
  }
  return 0;
}

