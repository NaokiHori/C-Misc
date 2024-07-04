#include <stdio.h>
#include <stdlib.h>
#include "combinations.h"

static int process (
    const size_t r,
    const size_t * combination
) {
  for (size_t i = 0; i < r; i++) {
    fprintf(stdout, "%zu%c", combination[i], i < r - 1 ? ' ' : '\n');
  }
  return 0;
}

int main (
    void
) {
  const size_t n = 6;
  const size_t r = 3;
  // store r indices
  size_t * combination = malloc(r * sizeof(size_t));
  // get initial combination
  init_combination(r, combination);
  process(r, combination);
  // find next combination until reached the last pair
  while (1) {
    if (0 != find_next_combination(n, r, combination)) {
      break;
    }
    process(r, combination);
  }
  free(combination);
  return 0;
}

