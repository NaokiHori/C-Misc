#if !defined(MYCOUNTER_H)
#define MYCOUNTER_H

typedef struct mycounter_t mycounter_t;

extern int mycounter_init (
    mycounter_t ** mycounter
);

extern int mycounter_add (
    mycounter_t * mycounter,
    const size_t item
);

extern int mycounter_get(
    const mycounter_t * mycounter,
    const size_t item,
    size_t * cntr
);

extern int mycounter_finalise (
    mycounter_t ** mycounter
);

#endif // MYCOUNTER_H
