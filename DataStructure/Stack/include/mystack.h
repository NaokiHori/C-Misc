#if !defined(MYSTACK_h)
#define MYSTACK_h

typedef struct mystack_t mystack_t;

extern int mystack_init (
    mystack_t ** mystack
);

extern int mystack_push (
    mystack_t * mystack,
    const size_t item
);

extern int mystack_peek (
    mystack_t * mystack,
    size_t * item
);

extern int mystack_count (
    mystack_t * mystack,
    size_t * nitems
);

extern int mystack_pop (
    mystack_t * mystack,
    size_t * item
);

extern int mystack_finalise (
    mystack_t ** mystack
);

#endif // MYSTACK_h
