#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "mypq.h"

// debug mode to check if heapified after all modifications, O(N)
static const bool debug = true;

// memory manager

static void * memory_alloc (
    const size_t size
) {
  void * ptr = malloc(size);
  if (NULL == ptr) {
    fprintf(stderr, "failed to allocate memory: %zu\n", size);
    exit(EXIT_FAILURE);
  }
  return ptr;
}

static void memory_free (
    void * ptr
) {
  free(ptr);
}

// priority queue

// minimum binary heap
struct mypq_t {
  size_t nitems;
  size_t ntags;
  mypq_tag_t * tags;
  bool (* l_is_less_than_or_equal_to_r) (const void * l, const void * r);
};

struct mypq_tag_t {
  void * user_item;
  mypq_tag_t ** self;
};

static size_t parent (
    const size_t index
) {
  return (index - 1) / 2;
}

static size_t l_child (
    const size_t index
) {
  return 2 * index + 1;
}

static size_t r_child (
    const size_t index
) {
  return 2 * index + 2;
}

static int swap (
    mypq_tag_t * tag0,
    mypq_tag_t * tag1
) {
  {
    mypq_tag_t tmp = *tag0;
    *tag0 = *tag1;
    *tag1 = tmp;
  }
  {
    mypq_tag_t * tmp = *(tag0->self);
    *(tag0->self) = *(tag1->self);
    *(tag1->self) = tmp;
  }
  return 0;
}

static const void * get_item (
    const size_t nitems,
    const mypq_tag_t * tags,
    const size_t index
) {
  return index < nitems ? tags[index].user_item : NULL;
}

static int check_heapified (
    const mypq_t * const mypq
) {
  if (!debug) {
    return 0;
  }
  const size_t nitems = mypq->nitems;
  const mypq_tag_t * tags = mypq->tags;
  bool (* const l_is_less_than_or_equal_to_r) (const void * l, const void * r) = mypq->l_is_less_than_or_equal_to_r;
  bool is_valid = true;
  // check child -> parent relations
  for (size_t index_c = 1; index_c < nitems; index_c++) {
    const size_t index_p = parent(index_c);
    const void * item_c = get_item(nitems, tags, index_c);
    const void * item_p = get_item(nitems, tags, index_p);
    if (l_is_less_than_or_equal_to_r(item_p, item_c)) {
      continue;
    }
    is_valid = false;
    fprintf(stderr, "c->p relation invalid at %zu\n", index_c);
  }
  // check parent -> children relations
  for (size_t index_p = 0; index_p < nitems; index_p++) {
    const size_t index_l = l_child(index_p);
    const size_t index_r = r_child(index_p);
    const void * item_p = get_item(nitems, tags, index_p);
    const void * item_l = get_item(nitems, tags, index_l);
    const void * item_r = get_item(nitems, tags, index_r);
    if (l_is_less_than_or_equal_to_r(item_p, item_l) && l_is_less_than_or_equal_to_r(item_p, item_r)) {
      continue;
    }
    is_valid = false;
    fprintf(stderr, "p->c relation is invalid at %zu\n", index_p);
  }
  if (is_valid) {
    return 0;
  } else {
    return 1;
  }
}

int mypq_init (
    mypq_t ** mypq,
    const size_t nitems,
    bool (* const l_is_less_than_or_equal_to_r) (const void * l, const void * r)
) {
  if (NULL != *mypq) {
    return 1;
  }
  *mypq = memory_alloc(sizeof(mypq_t));
  (*mypq)->nitems = nitems;
  (*mypq)->ntags = 0;
  (*mypq)->l_is_less_than_or_equal_to_r = l_is_less_than_or_equal_to_r;
  mypq_tag_t ** tags = &(*mypq)->tags;
  *tags = memory_alloc(nitems * sizeof(mypq_tag_t));
  for (size_t n = 0; n < nitems; n++) {
    mypq_tag_t * const tag = *tags + n;
    tag->self = NULL;
  }
  return 0;
}

int mypq_finalise (
    mypq_t ** mypq
) {
  if (NULL == *mypq) {
    return 1;
  }
  memory_free((*mypq)->tags);
  memory_free(*mypq);
  *mypq = NULL;
  return 0;
}

int mypq_register_item (
    mypq_t * mypq,
    mypq_tag_t ** self,
    void * user_item
) {
  if (NULL == mypq) {
    return 1;
  }
  mypq_tag_t * tags = mypq->tags;
  mypq_tag_t * tag = tags + mypq->ntags;
  *self = tag;
  tag->self = self;
  tag->user_item = user_item;
  mypq->ntags += 1;
  return 0;
}

int mypq_peek (
    mypq_t * mypq,
    void ** user_item
) {
  if (NULL == mypq) {
    return 1;
  }
  mypq_tag_t * tag = mypq->tags;
  if (NULL == tag) {
    return 1;
  }
  *user_item = tag->user_item;
  return 0;
}

int mypq_size (
    mypq_t * mypq,
    size_t * nitems
) {
  if (NULL == mypq) {
    return 1;
  }
  *nitems = mypq->nitems;
  return 0;
}

int mypq_update (
    mypq_t * mypq,
    mypq_tag_t ** self
) {
  if (NULL == mypq) {
    return 1;
  }
  const size_t nitems = mypq->nitems;
  if (nitems != mypq->ntags) {
    return 1;
  }
  mypq_tag_t * tags = mypq->tags;
  const size_t index = *self - tags;
  if (nitems - 1 < index) {
    return 1;
  }
  bool (* const l_is_less_than_or_equal_to_r) (const void * l, const void * r) = mypq->l_is_less_than_or_equal_to_r;
  // check relation with parent
  // if violated, upshift until heapified
  for (size_t index_c = index; 0 < index_c && index_c < nitems; ) {
    const size_t index_p = parent(index_c);
    const void * item_c = get_item(nitems, tags, index_c);
    const void * item_p = get_item(nitems, tags, index_p);
    if (l_is_less_than_or_equal_to_r(item_p, item_c)) {
      break;
    }
    swap(tags + index_c, tags + index_p);
    index_c = index_p;
  }
  // check relations with children
  // if violated, downshift until heapified
  for (size_t index_p = index; index_p < nitems; ) {
    const size_t index_l = l_child(index_p);
    const size_t index_r = r_child(index_p);
    const void * item_p = get_item(nitems, tags, index_p);
    const void * item_l = get_item(nitems, tags, index_l);
    const void * item_r = get_item(nitems, tags, index_r);
    if (l_is_less_than_or_equal_to_r(item_p, item_l) && l_is_less_than_or_equal_to_r(item_p, item_r)) {
      break;
    }
    // swap with smaller child
    if (l_is_less_than_or_equal_to_r(item_l, item_r)) {
      swap(tags + index_l, tags + index_p);
      index_p = index_l;
    } else {
      swap(tags + index_r, tags + index_p);
      index_p = index_r;
    }
  }
  return check_heapified(mypq);
}

#if defined(TEST_MYPQ)

#include <assert.h>

typedef struct {
  unsigned char priority;
  mypq_tag_t * mypq_tag;
} item_t;

static int test_swap (
    void
) {
  item_t items[] = {
    {
      .priority = 0,
      .mypq_tag = NULL,
    },
    {
      .priority = 1,
      .mypq_tag = NULL,
    },
  };
  mypq_tag_t mypq_tags[] = {
    {
      .user_item = NULL,
      .self = NULL,
    },
    {
      .user_item = NULL,
      .self = NULL,
    },
  };
  items[0].mypq_tag = mypq_tags + 0;
  items[1].mypq_tag = mypq_tags + 1;
  mypq_tags[0].user_item = &(items[0]);
  mypq_tags[1].user_item = &(items[1]);
  mypq_tags[0].self = &(items[0].mypq_tag);
  mypq_tags[1].self = &(items[1].mypq_tag);
  swap(mypq_tags + 0, mypq_tags + 1);
  // priorities are unaffected
  assert(0 == items[0].priority);
  assert(1 == items[1].priority);
  assert(0 == ((item_t *)items[0].mypq_tag->user_item)->priority);
  assert(1 == ((item_t *)items[1].mypq_tag->user_item)->priority);
  // user items are unaffected
  assert(items + 0 == items[0].mypq_tag->user_item);
  assert(items + 1 == items[1].mypq_tag->user_item);
  // connections are modified
  assert(mypq_tags + 1 == items[0].mypq_tag);
  assert(mypq_tags + 0 == items[1].mypq_tag);
  assert(mypq_tags + 1 == *(items[0].mypq_tag->self));
  assert(mypq_tags + 0 == *(items[1].mypq_tag->self));
  return 0;
}

int main (
    void
) {
  test_swap();
  return 0;
}

#endif // TEST_MYPQ
