#if !defined(MEMORY_H)
#define MEMORY_H

extern void * memory_alloc (
    const size_t nitems,
    const size_t size
);

extern void memory_free (
    void * ptr
);

#endif // MEMORY_H
