#if !defined(MYQUEUE_H)
#define MYQUEUE_H

typedef struct myqueue_t myqueue_t;

extern int myqueue_init (
    myqueue_t ** myqueue
);

extern int myqueue_enqueue (
    myqueue_t * myqueue,
    const size_t item
);

extern int myqueue_peek (
    myqueue_t * myqueue,
    size_t * item
);

extern int myqueue_count (
    myqueue_t * myqueue,
    size_t * nitems
);

extern int myqueue_dequeue (
    myqueue_t * queue,
    size_t * item
);

extern int myqueue_finalise (
    myqueue_t ** myqueue
);

#endif // MYQUEUE_H
