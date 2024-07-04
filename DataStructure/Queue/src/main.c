#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "myqueue.h"

static const size_t items[] = {0, 8, 2, 4, 8, 0, 8, 4, 3, 5, 5, 6, 3, 2, 8, 0};

#define MY_ASSERT(cond) \
  if (!(cond)) { \
    fprintf(stderr, "Line %d (function %s) failed\n", __LINE__, __func__); \
    return 1; \
  }

#define REPORT_SUCCESS(objective) fprintf(stderr, "Test success: %s (%s)\n", __func__, objective);

static int test0 (
    void
) {
  const char objective[] = {"all operations should fail before initialised"};
  myqueue_t * myqueue = NULL;
  size_t dummy = 0;
  MY_ASSERT(1 == myqueue_enqueue(myqueue, dummy));
  MY_ASSERT(1 == myqueue_peek(myqueue, &dummy));
  MY_ASSERT(1 == myqueue_count(myqueue, &dummy));
  MY_ASSERT(1 == myqueue_dequeue(myqueue, &dummy));
  MY_ASSERT(1 == myqueue_finalise(&myqueue));
  REPORT_SUCCESS(objective);
  return 0;
}

static int test1 (
    void
) {
  const char objective[] = {"finalisation immediately after initialisation"};
  myqueue_t * myqueue = NULL;
  MY_ASSERT(0 == myqueue_init(&myqueue));
  MY_ASSERT(0 == myqueue_finalise(&myqueue));
  MY_ASSERT(NULL == myqueue);
  REPORT_SUCCESS(objective);
  return 0;
}

static int test2 (
    void
) {
  const char objective[] = {"enqueue all, dequeue some, re-enqueue them, and dequeue all"};
  const size_t nitems = sizeof(items) / sizeof(items[0]);
  size_t counted_nitems = 0;
  size_t obtained_item = -1;
  // initialise
  myqueue_t * myqueue = NULL;
  MY_ASSERT(0 == myqueue_init(&myqueue));
  MY_ASSERT(0 == myqueue_count(myqueue, &counted_nitems));
  MY_ASSERT(0 == counted_nitems);
  // dequeue operation applied to an empty queeu should fail
  MY_ASSERT(1 == myqueue_dequeue(myqueue, &obtained_item));
  // enqueue all items: [nitems / 2 : nitems] -> [0 : nitems / 2]
  for (size_t n = 0; n < nitems / 2; n++) {
    MY_ASSERT(0 == myqueue_enqueue(myqueue, items[n + nitems / 2]));
    MY_ASSERT(0 == myqueue_peek(myqueue, &obtained_item));
    MY_ASSERT(obtained_item == items[nitems / 2]);
    MY_ASSERT(0 == myqueue_count(myqueue, &counted_nitems));
    MY_ASSERT(n + 1 == counted_nitems);
  }
  for (size_t n = 0; n < nitems / 2; n++) {
    MY_ASSERT(0 == myqueue_enqueue(myqueue, items[n]));
    MY_ASSERT(0 == myqueue_peek(myqueue, &obtained_item));
    MY_ASSERT(obtained_item == items[nitems / 2]);
    MY_ASSERT(0 == myqueue_count(myqueue, &counted_nitems));
    MY_ASSERT(n + 1 + nitems / 2 == counted_nitems);
  }
  // dequeue some items: [nitems / 2 : nitems]
  for (size_t n = 0; n < nitems / 2; n++) {
    MY_ASSERT(0 == myqueue_dequeue(myqueue, &obtained_item));
    MY_ASSERT(obtained_item == items[n + nitems / 2]);
    MY_ASSERT(0 == myqueue_count(myqueue, &counted_nitems));
    MY_ASSERT(nitems - n - 1 == counted_nitems);
  }
  // enqueue them again to make [0 : nitems]
  for (size_t n = 0; n < nitems / 2; n++) {
    MY_ASSERT(0 == myqueue_enqueue(myqueue, items[n + nitems / 2]));
    MY_ASSERT(0 == myqueue_peek(myqueue, &obtained_item));
    MY_ASSERT(obtained_item == items[0]);
    MY_ASSERT(0 == myqueue_count(myqueue, &counted_nitems));
    MY_ASSERT(n + 1 + nitems / 2 == counted_nitems);
  }
  // dequeue all items
  for (size_t n = 0; n < nitems; n++) {
    MY_ASSERT(0 == myqueue_dequeue(myqueue, &obtained_item));
    MY_ASSERT(obtained_item == items[n]);
    MY_ASSERT(0 == myqueue_count(myqueue, &counted_nitems));
    MY_ASSERT(nitems - n - 1 == counted_nitems);
  }
  // dequeue operation applied to an empty queeu should fail
  MY_ASSERT(1 == myqueue_dequeue(myqueue, &obtained_item));
  // finalise queue object
  MY_ASSERT(0 == myqueue_finalise(&myqueue));
  MY_ASSERT(NULL == myqueue);
  REPORT_SUCCESS(objective);
  return 0;
}

static int test3 (
    void
) {
  const char objective[] = {"[valgrind needed] does not leak memory"};
  const size_t nitems = sizeof(items) / sizeof(items[0]);
  myqueue_t * myqueue = NULL;
  // repeat twice
  for (size_t cnt = 0; cnt < 2; cnt++) {
    MY_ASSERT(0 == myqueue_init(&myqueue));
    for (size_t n = 0; n < nitems; n++) {
      MY_ASSERT(0 == myqueue_enqueue(myqueue, items[n]));
    }
    MY_ASSERT(0 == myqueue_finalise(&myqueue));
    MY_ASSERT(NULL == myqueue);
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

