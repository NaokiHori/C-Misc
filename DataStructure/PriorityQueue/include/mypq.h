#if !defined(MYPQ_H)
#define MYPQ_H

#include <stdbool.h>

typedef struct mypq_t mypq_t;
typedef struct mypq_tag_t mypq_tag_t;

extern int mypq_init (
    mypq_t ** mypq,
    const size_t nitems,
    bool (* l_is_less_than_or_equal_to_r) (
      const void * l,
      const void * r
    )
);

extern int mypq_finalise (
    mypq_t ** mypq
);

extern int mypq_register_item (
    mypq_t * mypq,
    mypq_tag_t ** self,
    void * user_item
);

extern int mypq_peek (
    mypq_t * mypq,
    void ** user_item
);

extern int mypq_size (
    mypq_t * mypq,
    size_t * nitems
);

extern int mypq_update (
    mypq_t * mypq,
    mypq_tag_t ** self
);

#endif // MYPQ_H
