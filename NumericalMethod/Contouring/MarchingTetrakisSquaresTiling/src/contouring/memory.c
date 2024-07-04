#include <stdio.h> // printf etc.
#include <stdlib.h> // malloc, free, exit, EXIT_FAILURE
#include "./memory.h"

void * memory_alloc (
    const size_t size
) {
  void * const ptr = malloc(size);
  if (NULL == ptr) {
    printf("failed to allocate %zu bytes\n", size);
    exit(EXIT_FAILURE);
  }
  return ptr;
}

void memory_free (
    void * const ptr
) {
  free(ptr);
}

