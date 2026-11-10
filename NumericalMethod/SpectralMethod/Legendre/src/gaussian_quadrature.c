#include <math.h>
#include <stdlib.h>
#include "./gaussian_quadrature/eigendecomposition.h"

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

// Golub-Welsch algorithm
int find_gaussian_quadrature(
    const size_t nitems,
    double * const roots,
    double * const weights
) {
  double * const main_diagonals = malloc(nitems * sizeof(double));
  double * const sub_diagonals = malloc((nitems - 1) * sizeof(double));
  for (size_t n = 0; n < nitems; n++) {
    main_diagonals[n] = 0.;
  }
  for (size_t n = 0; n < nitems - 1; n++) {
    sub_diagonals[n] = (1. + n) / sqrt(4. * pow(1. + n, 2.) - 1.);
  }
  double * const eigenvectors_transposed = malloc(nitems * nitems * sizeof(double));
  compute_eigendecomposition(
      nitems,
      main_diagonals,
      sub_diagonals,
      eigenvectors_transposed
  );
  sort_eigendecomposition(
      nitems,
      compare_eigen_pairs,
      main_diagonals,
      eigenvectors_transposed
  );
  for (size_t i = 0; i < nitems; i++) {
    const double root = 0.5 * main_diagonals[i] - 0.5 * main_diagonals[nitems - i - 1];
    const double weight =
      + pow(eigenvectors_transposed[i * nitems], 2.)
      + pow(eigenvectors_transposed[(nitems - i - 1) * nitems], 2.);
    roots[i] = root;
    weights[i] = weight;
  }
  free(main_diagonals);
  free(sub_diagonals);
  free(eigenvectors_transposed);
  return 0;
}

