#include <stddef.h>
#include "combinations.h"

// maximum possible value at index "i"
static inline size_t max (
    const size_t n,
    const size_t i
) {
  return n - 1 - i;
}

// set (r - 1, r - 2, ..., 1, 0)
int init_combination (
    const size_t r,
    size_t * combination
) {
  for (size_t i = 0; i < r; i++) {
    combination[i] = r - 1 - i;
  }
  return 0;
}

int find_next_combination (
    const size_t n,
    const size_t r,
    size_t * combination
) {
  // simply try to increase the first element
  // e.g., (2, 1, 0) -> (3, 1, 0)
  if (combination[0] < max(n, 0)) {
    combination[0] += 1;
    return 0;
  }
  // the first element has reached the maximum-allowed value already
  // we look for element whose index is "i", which can still be increased
  // e.g., n = 6, r = 3, (5, 1, 0) -> (3, 2, 0)
  //                     (5, 4, 0) -> (3, 2, 1)
  // sweeping from left to right
  for (size_t i = 1; i < r; i++) {
    if (combination[i] < max(n, i)) {
      // we found the i-th element has room for increase
      // increase it by 1 and set sequential numbers for the rest
      combination[i] += 1;
      for (size_t j = 0; j < i; j++) {
        combination[j] = combination[i] + i - j;
      }
      return 0;
    }
  }
  // reached the end (n - 1, n - 2, ..., n - r)
  return 1;
}

#if defined(TEST_COMBINATIONS)

#include <stdio.h>

#define MY_ASSERT(cond) \
  if (!(cond)) { \
    fprintf(stderr, "Line %d (function %s) failed\n", __LINE__, __func__); \
    return 1; \
  }

#define REPORT_SUCCESS(objective) fprintf(stderr, "Test success: %s (%s)\n", __func__, objective);

static int test0 (
    void
) {
  const char objective[] = {"init_combination"};
  {
#define R 1
    size_t combination[R] = {};
    init_combination(R, combination);
    MY_ASSERT(0 == combination[0]);
#undef R
  }
  {
#define R 2
    size_t combination[R] = {};
    init_combination(R, combination);
    MY_ASSERT(1 == combination[0]);
    MY_ASSERT(0 == combination[1]);
#undef R
  }
  {
#define R 4
    size_t combination[R] = {};
    init_combination(R, combination);
    MY_ASSERT(3 == combination[0]);
    MY_ASSERT(2 == combination[1]);
    MY_ASSERT(1 == combination[2]);
    MY_ASSERT(0 == combination[3]);
#undef R
  }
  REPORT_SUCCESS(objective);
  return 0;
}

static int test1 (
    void
) {
  const char objective[] = {"find_next_combination"};
  {
#define R 1
    const size_t n = 1;
    size_t combination[R] = {0};
    MY_ASSERT(1 == find_next_combination(n, R, combination));
#undef R
  }
  {
#define R 2
    const size_t n = 4;
    size_t combination[R] = {1, 0};
    MY_ASSERT(0 == find_next_combination(n, R, combination));
    MY_ASSERT(2 == combination[0] && 0 == combination[1]);
    MY_ASSERT(0 == find_next_combination(n, R, combination));
    MY_ASSERT(3 == combination[0] && 0 == combination[1]);
    MY_ASSERT(0 == find_next_combination(n, R, combination));
    MY_ASSERT(2 == combination[0] && 1 == combination[1]);
    MY_ASSERT(0 == find_next_combination(n, R, combination));
    MY_ASSERT(3 == combination[0] && 1 == combination[1]);
    MY_ASSERT(0 == find_next_combination(n, R, combination));
    MY_ASSERT(3 == combination[0] && 2 == combination[1]);
#undef R
  }
  REPORT_SUCCESS(objective);
  return 0;
}

int main (
    void
) {
  test0();
  test1();
  return 0;
}

#endif // TEST_COMBINATIONS

