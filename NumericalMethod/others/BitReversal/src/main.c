#include <stdio.h>
#include <stdlib.h>
#include "bit_reverse.h"

static int check (
    const char title[],
    const size_t nitems,
    const size_t * const items
) {
  printf("%s\n", title);
  for (size_t i = 0; i < nitems; i++) {
    printf("%4zu : %4zu\n", i, items[i]);
  }
  return 0;
}

int main (
    void
) {
  const size_t nitems = 16;
  size_t * const items = malloc(nitems * sizeof(size_t));
  for (size_t i = 0; i < nitems; i++) {
    items[i] = i;
  }
  check("bef", nitems, items);
  if (0 != bit_reverse(nitems, items)) {
    puts("bit_reverse failed");
    goto abort;
  }
  check("aft", nitems, items);
abort:
  free(items);
  return 0;
}

