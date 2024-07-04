#if !defined(MEMORY_H)
#define MEMORY_H

#include <stddef.h> // size_t

void * memory_alloc (
    const size_t size
);

void memory_free (
    void * const ptr
);

#endif // MEMORY_H
