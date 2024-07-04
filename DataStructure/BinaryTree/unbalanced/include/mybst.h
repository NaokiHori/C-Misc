#if !defined(MYBST_h)
#define MYBST_h

typedef struct mybst_t mybst_t;

extern int mybst_init (
    mybst_t ** mybst
);

extern int mybst_insert (
    mybst_t * mybst,
    const size_t item
);

extern int mybst_delete (
    mybst_t * mybst,
    const size_t item
);

extern int mybst_search (
    const mybst_t * mybst,
    const size_t item
);

extern int mybst_finalise (
    mybst_t ** mybst
);

#endif // MYBST_h
