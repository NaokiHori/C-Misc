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

static int decompose(
    const size_t nitems,
    double * const a,
    double * const q
) {
  // eliminate lower triangle
  for (size_t i = 0; i < nitems; i++) {
    for (size_t j = 0; j < i; j++) {
      rotate(nitems, i, j, a, q);
    }
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

int main(
    void
) {
#define NITEMS 3
  double matrices[2][NITEMS * NITEMS] = {
    // original matrix to be decomposed
    {
      6., 5., 0.,
      5., 1., 4.,
      0., 4., 3.,
    },
    // double buffer
    {
      0., 0., 0.,
      0., 0., 0.,
      0., 0., 0.,
    }
  };
  const double expected_eigenvalues[NITEMS] = {
    -3.864921474506954e+00,
    +4.021759941158753e+00,
    +9.843161533348200e+00,
  };
  double initial_frobenius_norm = 0.;
  compute_frobenius_norm(NITEMS, matrices[0], &initial_frobenius_norm);
  // qr algorithm
  // A^{k + 1} = R^k Q^k = QT^k A^k Q^k
  for (size_t iter = 0; iter < (1 << 10); iter++) {
    double q[NITEMS * NITEMS] = {
      1., 0., 0.,
      0., 1., 0.,
      0., 0., 1.,
    };
    double * const input = 0 == iter % 2 ? matrices[0] : matrices[1];
    double * const output = 0 == iter % 2 ? matrices[1] : matrices[0];
    decompose(NITEMS, input, q);
    compute_matrix_multiplication(NITEMS, input, q, output);
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
  qsort(eigenvalues, NITEMS, sizeof(double), compare_double_values);
  double error = 0.;
  for (size_t n = 0; n < NITEMS; n++) {
    error += pow(eigenvalues[n] - expected_eigenvalues[n], 2.);
  }
  printf("eigenvalue comparison: % .1e\n", sqrt(error / NITEMS));
  return 0;
}

