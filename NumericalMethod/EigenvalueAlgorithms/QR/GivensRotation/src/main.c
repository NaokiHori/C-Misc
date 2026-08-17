#include <math.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

static int print_matrix(
    const char label[],
    const size_t nitems,
    const double * const matrix
) {
  puts(label);
  for (size_t i = 0; i < nitems; i++) {
    for (size_t j = 0; j < nitems; j++) {
      printf("% .7f%c", matrix[i * nitems + j], nitems - 1 == j ? '\n' : ' ');
    }
  }
  puts("");
  return 0;
}

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
  print_matrix("A", nitems, a);
  print_matrix("Q", nitems, q);
  // eliminate lower triangle
  for (size_t i = 0; i < nitems; i++) {
    for (size_t j = 0; j < i; j++) {
      rotate(nitems, i, j, a, q);
    }
  }
  print_matrix("A", nitems, a);
  print_matrix("Q", nitems, q);
  return 0;
}

int main(
    void
) {
#define NITEMS 3
  // original matrix to be decomposed
  const double a[NITEMS * NITEMS] = {
    6., 5., 0.,
    5., 1., 4.,
    0., 4., 3.,
  };
  // construct copy of the matrix for later comparison
  double r[NITEMS * NITEMS] = {0.};
  for (size_t n = 0; n < NITEMS * NITEMS; n++) {
    r[n] = a[n];
  }
  // space to store orthogonal matrix
  double q[NITEMS * NITEMS] = {
    1., 0., 0.,
    0., 1., 0.,
    0., 0., 1.,
  };
  decompose(NITEMS, r, q);
  // assert decomposition
  // check A = Q R
  {
    double error = 0.;
    for (size_t i = 0; i < NITEMS; i++) {
      for (size_t j = 0; j < NITEMS; j++) {
        double solution_ij = 0.;
        for (size_t k = 0; k < NITEMS; k++) {
          solution_ij += q[i * NITEMS + k] * r[k * NITEMS + j];
        }
        error += pow(solution_ij - a[i * NITEMS + j], 2.);
      }
    }
    printf("L^2 error (A = Q R): % .1e\n", sqrt(error / NITEMS / NITEMS));
  }
  // check orthogonality of the vectors
  {
    double error = 0.;
    for (size_t i = 0; i < NITEMS; i++) {
      for (size_t j = 0; j < i; j++) {
        double inner_product = 0.;
        for (size_t k = 0; k < NITEMS; k++) {
          inner_product += q[k * NITEMS + i] * q[k * NITEMS + j];
        }
        error += pow(inner_product, 2.);
      }
    }
    printf("L^2 error (0 = Qu Qv): % .1e\n", sqrt(error / NITEMS / (NITEMS - 1) * 2));
  }
  // check norm of each vector
  {
    double error = 0.;
    for (size_t j = 0; j < NITEMS; j++) {
      double inner_product = 0.;
      for (size_t i = 0; i < NITEMS; i++) {
        inner_product += pow(q[i * NITEMS + j], 2.);
      }
      error += pow(1. - inner_product, 2.);
    }
    printf("L^2 error (1 = Qu Qu): % .1e\n", sqrt(error / NITEMS));
  }
  return 0;
}

