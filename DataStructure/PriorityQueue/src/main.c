#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "mypq.h"

#define MY_ASSERT(cond) \
  if (!(cond)) { \
    fprintf(stderr, "Line %d (function %s) failed\n", __LINE__, __func__); \
    return 1; \
  }

#define REPORT_SUCCESS(objective) fprintf(stderr, "Test success: %s (%s)\n", __func__, objective);

static const size_t priority_infty = SIZE_MAX;

typedef struct item_t {
  size_t priority;
  mypq_tag_t * mypq_tag;
} item_t;

static bool l_is_less_than_or_equal_to_r (
    const void * l,
    const void * r
) {
  const item_t * item_l = (item_t *)l;
  const item_t * item_r = (item_t *)r;
  const size_t priority_l = item_l ? item_l->priority : priority_infty;
  const size_t priority_r = item_r ? item_r->priority : priority_infty;
  if (priority_l <= priority_r) {
    return true;
  } else {
    return false;
  }
}

static int test0 (
    void
) {
  const char objective[] = {"all operations should fail before initialised"};
  mypq_t * mypq = NULL;
  mypq_tag_t * mypq_tag = NULL;
  void * item = NULL;
  size_t nitems = 0;
  MY_ASSERT(1 == mypq_register_item(mypq, &mypq_tag, item));
  MY_ASSERT(1 == mypq_peek(mypq, &item));
  MY_ASSERT(1 == mypq_size(mypq, &nitems));
  MY_ASSERT(1 == mypq_finalise(&mypq));
  MY_ASSERT(NULL == mypq);
  REPORT_SUCCESS(objective);
  return 0;
}

static int test1 (
    void
) {
  const char objective[] = {"finalisation immediately after initialisation"};
  const size_t nitems = 32;
  mypq_t * mypq = NULL;
  MY_ASSERT(0 == mypq_init(&mypq, nitems, l_is_less_than_or_equal_to_r));
  MY_ASSERT(0 == mypq_finalise(&mypq));
  MY_ASSERT(NULL == mypq);
  REPORT_SUCCESS(objective);
  return 0;
}

static int test2 (
    void
) {
  const char objective[] = {"normal use"};
  const size_t nitems = 128;
  // prepare queue main object
  mypq_t * mypq = NULL;
  MY_ASSERT(0 == mypq_init(&mypq, nitems, l_is_less_than_or_equal_to_r));
  // init items and link each item to the queue
  item_t * items = malloc(nitems * sizeof(item_t));
  for (size_t n = 0; n < nitems; n++) {
    item_t * const item = items + n;
    item->priority = priority_infty;
    MY_ASSERT(0 == mypq_register_item(mypq, &item->mypq_tag, item));
    if (n < nitems - 1) {
      // should fail to update queue unless all items are registered
      MY_ASSERT(1 == mypq_update(mypq, &item->mypq_tag));
    }
  }
  // assign initial priorities
  for (size_t n = 0; n < nitems; n++) {
    item_t * const item = items + n;
    item->priority = rand();
    MY_ASSERT(0 == mypq_update(mypq, &item->mypq_tag));
  }
  // update random item for several times
  const size_t cntmax = 1024;
  for (size_t cnt = 0; cnt < cntmax; cnt++) {
    const size_t index = rand() % nitems;
    item_t * const item = items + index;
    item->priority = rand();
    MY_ASSERT(0 == mypq_update(mypq, &item->mypq_tag));
  }
  // keep peeking, previous priority should be smaller than or equal to the current one
  for (size_t prev_priority = priority_infty, n = 0; n < nitems; n++) {
    item_t * item = NULL;
    MY_ASSERT(0 == mypq_peek(mypq, (void **)&item));
    const size_t curr_priority = item->priority;
    if (0 < n) {
      MY_ASSERT(prev_priority <= curr_priority);
    }
    prev_priority = curr_priority;
    item->priority = priority_infty;
    MY_ASSERT(0 == mypq_update(mypq, &item->mypq_tag));
  }
  // clean-up all
  free(items);
  MY_ASSERT(0 == mypq_finalise(&mypq));
  MY_ASSERT(NULL == mypq);
  REPORT_SUCCESS(objective);
  return 0;
}

static int test3 (
    void
) {
  const char objective[] = {"[valgrind needed] does not leak memory"};
  const size_t nitems = 8;
  mypq_t * mypq = NULL;
  // repeat twice
  for (size_t cnt = 0; cnt < 2; cnt++) {
    MY_ASSERT(0 == mypq_init(&mypq, nitems, l_is_less_than_or_equal_to_r));
    item_t * items = malloc(nitems * sizeof(item_t));
    for (size_t n = 0; n < nitems; n++) {
      item_t * const item = items + n;
      item->priority = priority_infty;
      MY_ASSERT(0 == mypq_register_item(mypq, &item->mypq_tag, item));
    }
    free(items);
    MY_ASSERT(0 == mypq_finalise(&mypq));
    MY_ASSERT(NULL == mypq);
  }
  REPORT_SUCCESS(objective);
  return 0;
}

int main (
    void
) {
  test0();
  test1();
  test2();
  test3();
  return 0;
}

