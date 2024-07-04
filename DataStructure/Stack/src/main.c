#include <stdio.h>
#include <stdlib.h>
#include "mystack.h"

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
  mystack_t * mystack = NULL;
  size_t dummy = 0;
  MY_ASSERT(1 == mystack_push(mystack, dummy));
  MY_ASSERT(1 == mystack_peek(mystack, &dummy));
  MY_ASSERT(1 == mystack_count(mystack, &dummy));
  MY_ASSERT(1 == mystack_pop(mystack, &dummy));
  MY_ASSERT(1 == mystack_finalise(&mystack));
  REPORT_SUCCESS(objective);
  return 0;
}

static int test1 (
    void
) {
  const char objective[] = {"finalisation immediately after initialisation"};
  mystack_t * mystack = NULL;
  MY_ASSERT(0 == mystack_init(&mystack));
  MY_ASSERT(0 == mystack_finalise(&mystack));
  MY_ASSERT(NULL == mystack);
  REPORT_SUCCESS(objective);
  return 0;
}

static int test2 (
    void
) {
  const char objective[] = {"push all, pop some, re-push them, and pop all"};
  const size_t nitems = sizeof(items) / sizeof(items[0]);
  size_t counted_nitems = 0;
  size_t obtained_item = -1;
  // initialise
  mystack_t * mystack = NULL;
  MY_ASSERT(0 == mystack_init(&mystack));
  MY_ASSERT(0 == mystack_count(mystack, &counted_nitems));
  MY_ASSERT(0 == counted_nitems);
  // pop operation applied to an empty stack should fail
  MY_ASSERT(1 == mystack_pop(mystack, &obtained_item));
  // push all items
  for (size_t n = 0; n < nitems; n++) {
    MY_ASSERT(0 == mystack_push(mystack, items[n]));
    MY_ASSERT(0 == mystack_peek(mystack, &obtained_item));
    MY_ASSERT(obtained_item == items[n]);
    MY_ASSERT(0 == mystack_count(mystack, &counted_nitems));
    MY_ASSERT(n + 1 == counted_nitems);
  }
  // pop some items
  for (size_t n = 0; n < nitems / 2; n++) {
    MY_ASSERT(0 == mystack_pop(mystack, &obtained_item));
    MY_ASSERT(obtained_item == items[nitems - n - 1]);
    MY_ASSERT(0 == mystack_count(mystack, &counted_nitems));
    MY_ASSERT(nitems - n - 1 == counted_nitems);
  }
  // push them again
  for (size_t n = nitems / 2; n < nitems; n++) {
    MY_ASSERT(0 == mystack_push(mystack, items[n]));
    MY_ASSERT(0 == mystack_peek(mystack, &obtained_item));
    MY_ASSERT(obtained_item == items[n]);
    MY_ASSERT(0 == mystack_count(mystack, &counted_nitems));
    MY_ASSERT(n + 1 == counted_nitems);
  }
  // pop all items
  for (size_t n = 0; n < nitems; n++) {
    MY_ASSERT(0 == mystack_pop(mystack, &obtained_item));
    MY_ASSERT(obtained_item == items[nitems - n - 1]);
    MY_ASSERT(0 == mystack_count(mystack, &counted_nitems));
    MY_ASSERT(nitems - n - 1 == counted_nitems);
  }
  // pop operation applied to an empty stack should fail
  MY_ASSERT(1 == mystack_pop(mystack, &obtained_item));
  // finalise stack object
  MY_ASSERT(0 == mystack_finalise(&mystack));
  MY_ASSERT(NULL == mystack);
  REPORT_SUCCESS(objective);
  return 0;
}

static int test3 (
    void
) {
  const char objective[] = {"[valgrind needed] does not leak memory"};
  const size_t nitems = sizeof(items) / sizeof(items[0]);
  mystack_t * mystack = NULL;
  // repeat twice
  for (size_t cnt = 0; cnt < 2; cnt++) {
    MY_ASSERT(0 == mystack_init(&mystack));
    for (size_t n = 0; n < nitems; n++) {
      MY_ASSERT(0 == mystack_push(mystack, items[n]));
    }
    MY_ASSERT(0 == mystack_finalise(&mystack));
    MY_ASSERT(NULL == mystack);
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

