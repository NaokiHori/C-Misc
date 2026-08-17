#include <math.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

// compute Wilkinson's shift
static int compute_shift(
    const double * const main_diagonals,
    const double * const sub_diagonals,
    const size_t n_end,
    double * const shift
) {
  if (n_end < 2) {
    return 1;
  }
  const double diagonals[2] = {
    main_diagonals[n_end - 2],
    main_diagonals[n_end - 1],
  };
  const double sub_diagonal = sub_diagonals[n_end - 2];
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

static int chase_bulge(
    const size_t nitems,
    const size_t n_end,
    const double shift,
    double * const main_diagonals,
    double * const sub_diagonals,
    double * const eigenvectors_transposed
) {
  // rotation angle is determined by these quantities
  double values[2] = {
    main_diagonals[0] - shift,
    sub_diagonals[0],
  };
  for (size_t i = 0; i < n_end - 1; i++) {
    // Givens rotation applied to i-th and (i+1)-th rows
    // BEFORE
    //   (i    )-th row: [ . . s m s . . . ]
    //   (i + 1)-th row: [ . . b s m s . . ]
    // AFTER
    //   (i    )-th row: [ . . s m s b . . ]
    //   (i + 1)-th row: [ . . . s m s . . ]
    // - m: main-diagonal
    // - s: sub-diagonal
    // - b: bulge
    const double r = hypot(values[0], values[1]);
    if (0 < i) {
      // affected by eliminating the bulge
      sub_diagonals[i - 1] = r;
    }
    const double c = 0. == r ? 1. : + values[0] / r;
    const double s = 0. == r ? 0. : - values[1] / r;
    const double local_main_diagonals[2] = {
      main_diagonals[i    ],
      main_diagonals[i + 1],
    };
    const double local_sub_diagonal = sub_diagonals[i];
    // update the 2x2 diagonal block this rotation acts on
    // Q^T A Q = G A G^T =
    //   [  c -s ] [ d0  e ] [  c  s ]
    //   [  s  c ] [  e d1 ] [ -s  c ]
    const double cc = c * c;
    const double sc = s * c;
    const double ss = s * s;
    main_diagonals[i    ] = cc * local_main_diagonals[0] - 2. * sc * local_sub_diagonal + ss * local_main_diagonals[1];
    main_diagonals[i + 1] = ss * local_main_diagonals[0] + 2. * sc * local_sub_diagonal + cc * local_main_diagonals[1];
    sub_diagonals[i] = sc * local_main_diagonals[0] + (cc - ss) * local_sub_diagonal - sc * local_main_diagonals[1];
    // update (transposed) eigenvectors
    // V := V Q = V G^T
    // -> V^T := (V G^T)^T = G V^T
    for (size_t j = 0; j < nitems; j++) {
      // for each column (j), two rows (i and i + 1) are modulated
      double * const elements[2] = {
        eigenvectors_transposed + (i    ) * nitems + j,
        eigenvectors_transposed + (i + 1) * nitems + j,
      };
      const double values[2] = {
        *elements[0],
        *elements[1],
      };
      *elements[0] = + c * values[0] - s * values[1];
      *elements[1] = + s * values[0] + c * values[1];
    }
    if (n_end - 2 == i) {
      break;
    }
    // prepare for the next bulge chasing
    // [ b ] = [  c -s ] [ 0 ] = [ -s * e ]
    // [ s ] = [  s  c ] [ e ] = [  c * e ]
    values[0] = sub_diagonals[i];
    // new bulge
    values[1] = - s * sub_diagonals[i + 1];
    sub_diagonals[i + 1] *= c;
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

static int compute_l2_sub_diagonals(
    const size_t nitems,
    const double * const sub_diagonals,
    double * const difference
) {
  if (nitems < 1) {
    return 1;
  }
  *difference = 0.;
  for (size_t n = 0; n < nitems - 1; n++) {
    *difference += pow(sub_diagonals[n], 2.);
  }
  *difference = sqrt(*difference / (nitems - 1));
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
  build_identity_matrix(NITEMS, eigenvectors_transposed);
  size_t n_end = NITEMS;
  // for safety: should not reach
  const size_t iter_max = NITEMS * 4;
  for (size_t iter = 0;;) {
    double shift = 0.;
    compute_shift(main_diagonals, sub_diagonals, n_end, &shift);
    chase_bulge(NITEMS, n_end, shift, main_diagonals, sub_diagonals, eigenvectors_transposed);
    iter += 1;
    //
    double l2_sub_diagonals = 0.;
    compute_l2_sub_diagonals(
        NITEMS,
        sub_diagonals,
        &l2_sub_diagonals
    );
    const double target_sub_diagonal = sub_diagonals[n_end - 2];
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
      printf("converged after %zu iterations\n", iter);
      check_eigendecomposition(
          NITEMS,
          original_main_diagonals,
          original_sub_diagonals,
          expected_eigenvalues,
          main_diagonals,
          eigenvectors_transposed
      );
      break;
    }
    if (iter_max < iter) {
      printf("not converged after %zu iterations", iter);
      break;
    }
  }
  return 0;
}

