#include <stdio.h>
#include <stdlib.h>
#include "mycounter.h"

#define MY_ASSERT(cond) \
  if (!(cond)) { \
    fprintf(stderr, "Line %d (function %s) failed\n", __LINE__, __func__); \
    return 1; \
  }

#define REPORT_SUCCESS(objective) fprintf(stderr, "Test success: %s (%s)\n", __func__, objective);

static size_t find_max_value (
    const size_t nitems,
    const size_t * items
) {
  size_t max_value = 0;
  for (size_t n = 0; n < nitems; n++) {
    const size_t item = items[n];
    max_value = max_value < item ? item : max_value;
  }
  return max_value;
}

static size_t check_counter (
    const size_t nitems,
    const size_t * items,
    const size_t item
) {
  size_t counter = 0;
  for (size_t n = 0; n < nitems; n++) {
    if (item == items[n]) {
      counter += 1;
    }
  }
  return counter;
}

static int test0 (
    void
) {
  const char objective[] = {"all operations should fail before initialised"};
  mycounter_t * mycounter = NULL;
  size_t dummy = 0;
  MY_ASSERT(1 == mycounter_add(mycounter, dummy));
  MY_ASSERT(1 == mycounter_get(mycounter, dummy, &dummy));
  MY_ASSERT(1 == mycounter_finalise(&mycounter));
  REPORT_SUCCESS(objective);
  return 0;
}

static int test1 (
    void
) {
  const char objective[] = {"finalisation immediately after initialisation"};
  mycounter_t * mycounter = NULL;
  MY_ASSERT(0 == mycounter_init(&mycounter));
  MY_ASSERT(0 == mycounter_finalise(&mycounter));
  MY_ASSERT(NULL == mycounter);
  REPORT_SUCCESS(objective);
  return 0;
}

static int test2 (
    void
) {
  const char objective[] = {"normal use"};
  const size_t nitems = 100000;
  size_t * items = malloc(nitems * sizeof(size_t));
  for (size_t n = 0; n < nitems; n++) {
    items[n] = rand() % 1024;
  }
  mycounter_t * mycounter = NULL;
  MY_ASSERT(0 == mycounter_init(&mycounter));
  // add all items
  for (size_t n = 0; n < nitems; n++) {
    MY_ASSERT(0 == mycounter_add(mycounter, items[n]));
  }
  // compare with answer
  for (size_t item = 0; item < find_max_value(nitems, items); item++) {
    const size_t expected = check_counter(nitems, items, item);
    size_t obtained = -1;
    MY_ASSERT(0 == mycounter_get(mycounter, item, &obtained));
    MY_ASSERT(expected == obtained);
  }
  MY_ASSERT(0 == mycounter_finalise(&mycounter));
  MY_ASSERT(NULL == mycounter);
  free(items);
  REPORT_SUCCESS(objective);
  return 0;
}

int main (
    void
) {
  test0();
  test1();
  test2();
  return 0;
}

