#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>
#include "matrix.h"

static int print(
  const char title[],
  const size_t nitems,
  rational_t * const matrix
) {
  printf("%s\n", title);
  for (size_t i = 0; i < nitems; i++) {
    for (size_t j = 0; j < nitems; j++) {
      const rational_t * const value = matrix + i * nitems + j;
      printf("(%4lld/%4lld)%c", value->numerator, value->denominator, nitems - 1 == j ? '\n' : ' ');
    }
  }
  return 0;
}

static int check(
  const size_t nitems,
  const rational_t * const matrix_a,
  const rational_t * const matrix_b
) {
  rational_t * matrix_c = malloc(nitems * nitems * sizeof(rational_t));
  for (size_t i = 0; i < nitems; i++) {
    for (size_t j = 0; j < nitems; j++) {
      rational_t * const value_c = matrix_c + i * nitems + j;
      value_c->numerator = 0;
      value_c->denominator = 1;
      for (size_t k = 0; k < nitems; k++) {
        const rational_t * const value_a = matrix_a + i * nitems + k;
        const rational_t * const value_b = matrix_b + k * nitems + j;
        const rational_t multiplied = multiply(value_a, value_b);
        *value_c = add(value_c, &multiplied);
      }
    }
  }
  print("expect identity matrix", nitems, matrix_c);
  return 0;
}

int main(
  void
) {
  const size_t nitems = 3;
  rational_t * const matrix = malloc(nitems * nitems * sizeof(rational_t));
  rational_t * const original = malloc(nitems * nitems * sizeof(rational_t));
  for (size_t i = 0; i < nitems; i++) {
    for (size_t j = 0; j < nitems; j++) {
      rational_t * const value = matrix + i * nitems + j;
      if (i == j) {
        value->numerator = -2;
        value->denominator = 1;
      } else if (i + 1 == j || j + 1 == i) {
        value->numerator = 1;
        value->denominator = 1;
      } else {
        value->numerator = 0;
        value->denominator = 1;
      }
    }
  }
  print("original matrix", nitems, matrix);
  for (size_t i = 0; i < nitems; i++) {
    for (size_t j = 0; j < nitems; j++) {
      *(original + i * nitems + j) = *(matrix + i * nitems + j);
    }
  }
  rational_t * const inverse = malloc(nitems * nitems * sizeof(rational_t));
  if (0 != compute_inverse_matrix(nitems, matrix, inverse)) {
    print("singular matrix", nitems, matrix);
    return 1;
  }
  print("matrix inverse", nitems, inverse);
  check(nitems, original, inverse);
  free(matrix);
  free(original);
  free(inverse);
  return 0;
}
