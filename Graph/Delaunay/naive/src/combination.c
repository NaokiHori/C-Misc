#include "./combination.h"

static inline size_t max(
    const size_t n,
    const size_t i
) {
  return n - 1 - i;
}

int init_combination(
    const size_t r,
    size_t * const combination
) {
  for (size_t i = 0; i < r; i++) {
    combination[i] = r - 1 - i;
  }
  return 0;
}

int find_next_combination(
    const size_t n,
    const size_t r,
    size_t * const combination
) {
  if (combination[0] < max(n, 0)) {
    combination[0] += 1;
    return 0;
  }
  for (size_t i = 1; i < r; i++) {
    if (combination[i] < max(n, i)) {
      combination[i] += 1;
      for (size_t j = 0; j < i; j++) {
        combination[j] = combination[i] + i - j;
      }
      return 0;
    }
  }
  return 1;
}

