#include <stdio.h>
#include "bit_reverse.h"

static size_t get_nbits (
    size_t n
) {
  size_t nbits = 0;
  while (n >>= 1) {
    nbits += 1;
  }
  return nbits;
}

static size_t get_reversed_bit_index (
    const size_t nitems,
    size_t n
) {
  const size_t nbits = get_nbits(nitems);
  size_t retval = 0;
  for (size_t i = 0; i < nbits; i++) {
    retval <<= 1;
    retval |= n & 1;
    n >>= 1;
  }
  return retval;
}

int bit_reverse (
    const size_t nitems,
    size_t * const items
) {
  if ((nitems - 1) & nitems) {
    printf("nitems (%zu) should be a power of 2\n", nitems);
    return 1;
  }
  for (size_t i = 0; i < nitems; i++) {
    const size_t j = get_reversed_bit_index(nitems, i);
    if (i < j) {
      const size_t tmp = items[i];
      items[i] = items[j];
      items[j] = tmp;
    }
  }
  return 0;
}

