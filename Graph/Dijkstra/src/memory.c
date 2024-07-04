#include <stdio.h>
#include <stdlib.h>
#include "memory.h"

void * memory_alloc (
    const size_t nitems,
    const size_t size
) {
  void * ptr = malloc(nitems * size);
  if (NULL == ptr) {
    printf("FATAL: failed to allocate %zu x %zu bytes\n", nitems, size);
    exit(EXIT_FAILURE);
  }
  return ptr;
}

void memory_free (
    void * ptr
) {
  free(ptr);
}

