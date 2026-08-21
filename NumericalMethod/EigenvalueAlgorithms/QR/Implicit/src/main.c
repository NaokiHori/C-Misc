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
    const size_t n_end,
    const double shift,
    double * const main_diagonals,
    double * const sub_diagonals
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

static int compare_l2_eigenvalues(
    const size_t nitems,
    const double * const expected_eigenvalues,
    const double * const main_diagonals,
    double * const error
) {
  double * const eigenvalues = malloc(nitems * sizeof(double));
  for (size_t n = 0; n < nitems; n++) {
    eigenvalues[n] = main_diagonals[n];
  }
  qsort(eigenvalues, nitems, sizeof(double), compare_double_values);
  *error = 0.;
  for (size_t n = 0; n < nitems; n++) {
    *error += pow(eigenvalues[n] - expected_eigenvalues[n], 2.);
  }
  *error = sqrt(*error / nitems);
  free(eigenvalues);
  return 0;
}

int main(
    void
) {
#define NITEMS 128
  double main_diagonals[NITEMS] = {0.};
  double sub_diagonals[NITEMS - 1] = {0.};
  double expected_eigenvalues[NITEMS] = {0.};
  build_tridiagonal_matrix(NITEMS, -2., 1., main_diagonals, sub_diagonals, expected_eigenvalues);
  // qr algorithm
  // A^{k + 1} = R^k Q^k = QT^k A^k Q^k
  size_t n_end = NITEMS;
  // for safety: should not reach
  const size_t iter_max = NITEMS * 4;
  for (size_t iter = 0;;) {
    double shift = 0.;
    compute_shift(main_diagonals, sub_diagonals, n_end, &shift);
    chase_bulge(n_end, shift, main_diagonals, sub_diagonals);
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
      double l2_eigenvalues = 0.;
      compare_l2_eigenvalues(NITEMS, expected_eigenvalues, main_diagonals, &l2_eigenvalues);
      printf("converged, eigenvalue error: % .1e\n", l2_eigenvalues);
      break;
    }
    if (iter_max < iter) {
      double l2_eigenvalues = 0.;
      compare_l2_eigenvalues(NITEMS, expected_eigenvalues, main_diagonals, &l2_eigenvalues);
      printf("not converged after %zu iterations, eigenvalue error: % .1e\n", iter_max, l2_eigenvalues);
      break;
    }
  }
  return 0;
}

