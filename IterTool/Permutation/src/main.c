#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

static int get_permutation(
    const size_t nitems,
    size_t * const vector
) {
  // flag to check the number has already been eaten-up,
  //   initially false of course
  bool * const used = malloc(nitems * sizeof(bool));
  if (NULL == used) {
    puts("malloc error");
    return 1;
  }
  for (size_t n = 0; n < nitems; n++) {
    used[n] = false;
  }
  // decide n-th element in vector
  for (size_t n = 0; n < nitems; n++) {
    // random value between 0 and (nitems - n - 1)
    const size_t r = rand() % (nitems - n);
    // pick up r-th value, but I need skip numbers
    //   which have already been used
    for (size_t cnt = 0, m = 0; m < nitems; m++) {
      // skip if m-th value is already consumed
      if (used[m]) {
        continue;
      }
      // now m-th value is unused
      // when the counter matches with r,
      //   this is what I should pick up now
      if (r == cnt) {
        used[m] = true;
        vector[n] = m;
        break;
      }
      // otherwise just increment counter
      cnt += 1;
    }
  }
  free(used);
  return 0;
}

int main(
    void
) {
  int retval = 0;
  srand(0);
  const size_t nitems = 9;
  size_t * const vector = malloc(nitems * sizeof(size_t));
  if (NULL == vector) {
    puts("malloc error");
    retval = 1;
    goto abort;
  }
  // repeat generation process for several times
  for (size_t iter = 0; iter < 10; iter++) {
    // get one of the permutations randomly
    if (0 != get_permutation(nitems, vector)) {
      puts("failed to get permutation");
      retval = 1;
      goto abort;
    }
    // dump result vector
    printf("attempt %zu: ", iter);
    for (size_t n = 0; n < nitems; n++) {
      printf("%3zu%c", vector[n], nitems - 1 == n ? '\n' : ' ');
    }
  }
abort:
  free(vector);
  return retval;
}

