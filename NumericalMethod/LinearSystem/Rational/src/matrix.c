#include "matrix.h"

// NOTE: overflow not considered

static int invert(
  const rational_t * const value,
  rational_t * inverted
) {
  if (0 == value->numerator) {
    return 1;
  }
  inverted->numerator = value->denominator;
  inverted->denominator = value->numerator;
  return 0;
}

static int64_t abs(
  const int64_t value
) {
  return value < 0 ? - value : value;
}

static int64_t compute_gcd(
  int64_t a,
  int64_t b
) {
  for (;;) {
    const int64_t abs_a = abs(a);
    const int64_t abs_b = abs(b);
    const int64_t c = abs_a < abs_b ? a : b;
    const int64_t d = abs_a < abs_b ? b : a;
    const int64_t r = d % c;
    if (0 == r) {
      return c;
    } else {
      a = c;
      b = r;
    }
  }
}

static int simplify(
  rational_t * const value
) {
  if (0 == value->numerator) {
    value->denominator = 1;
    return 0;
  }
  const int64_t gcd = compute_gcd(value->numerator, value->denominator);
  value->numerator /= gcd;
  value->denominator /= gcd;
  if (value->denominator < 0) {
    value->numerator *= -1;
    value->denominator *= -1;
  }
  return 0;
}

rational_t add(
  const rational_t * const a,
  const rational_t * const b
) {
  rational_t result = (rational_t){
    .numerator = a->numerator * b->denominator + a->denominator * b->numerator,
    .denominator = a->denominator * b->denominator,
  };
  simplify(&result);
  return result;
}

rational_t subtract(
  const rational_t * const a,
  const rational_t * const b
) {
  rational_t result = (rational_t){
    .numerator = a->numerator * b->denominator - a->denominator * b->numerator,
    .denominator = a->denominator * b->denominator,
  };
  simplify(&result);
  return result;
}

rational_t multiply(
  const rational_t * const a,
  const rational_t * const b
) {
  rational_t result = (rational_t){
    .numerator = a->numerator * b->numerator,
    .denominator = a->denominator * b->denominator,
  };
  simplify(&result);
  return result;
}

int compute_inverse_matrix(
  const size_t nitems,
  rational_t * const matrix_a,
  rational_t * const matrix_b
) {
  // initialize as an identity matrix
  for (size_t i = 0; i < nitems; i++) {
    for (size_t j = 0; j < nitems; j++) {
      matrix_b[i * nitems + j].numerator = i == j ? 1 : 0;
      matrix_b[i * nitems + j].denominator = 1;
    }
  }
  // forward sweep
  for (size_t n = 0; n < nitems; n++) {
    rational_t factor = {0};
    if (0 != invert(matrix_a + n * nitems + n, &factor)) {
      return 1;
    }
    for (size_t j = n; j < nitems; j++) {
      rational_t * const value = matrix_a + n * nitems + j;
      *value = multiply(&factor, value);
    }
    for (size_t j = 0; j < n + 1; j++) {
      rational_t * const value = matrix_b + n * nitems + j;
      *value = multiply(&factor, value);
    }
    for (size_t i = n + 1; i < nitems; i++) {
      const rational_t factor = *(matrix_a + i * nitems + n);
      for (size_t j = n; j < nitems; j++) {
        const rational_t intermediate = multiply(&factor, matrix_a + n * nitems + j);
        rational_t * const value = matrix_a + i * nitems + j;
        *value = subtract(value, &intermediate);
      }
      for (size_t j = 0; j < n + 1; j++) {
        const rational_t intermediate = multiply(&factor, matrix_b + n * nitems + j);
        rational_t * const value = matrix_b + i * nitems + j;
        *value = subtract(value, &intermediate);
      }
    }
  }
  // backward substitution
  for (size_t n = 1; n < nitems; n++) {
    for (size_t i = 0; i < n; i++) {
      const rational_t factor = matrix_a[i * nitems + n];
      for (size_t j = n; j < nitems; j++) {
        rational_t * const value_a = matrix_a + i * nitems + j;
        rational_t * const value_b = matrix_a + n * nitems + j;
        const rational_t value_c = multiply(value_b, &factor);
        *value_a = subtract(value_a, &value_c);
      }
      for (size_t j = 0; j < nitems; j++) {
        rational_t * const value_a = matrix_b + i * nitems + j;
        rational_t * const value_b = matrix_b + n * nitems + j;
        const rational_t value_c = multiply(value_b, &factor);
        *value_a = subtract(value_a, &value_c);
      }
    }
  }
  return 0;
}

