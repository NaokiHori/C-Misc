#include <stdio.h>
#include <stdlib.h>
#include "mybst.h"

#define MY_ASSERT(cond) \
  if (!(cond)) { \
    fprintf(stderr, "Line %d (function %s) failed\n", __LINE__, __func__); \
    return 1; \
  }

#define REPORT_SUCCESS(objective) fprintf(stderr, "Test success: %s (%s)\n", __func__, objective);

static const size_t items[] = {38, 84, 83, 35, 96, 21, 88, 59, 107, 18, 62, 126, 124, 75, 56, 98, 47, 49, 63, 51, 119, 102, 78, 19, 104, 92, 32, 67, 123, 82, 95, 26, 20, 86, 81, 110, 90, 33, 58, 115, 79, 22, 87, 64, 66, 60, 6, 73, 91, 36, 109, 116, 52, 50, 101, 34, 71, 65, 11, 17, 93, 16, 48, 13, 15, 41, 9, 29, 44, 40, 45, 112, 27, 117, 105, 120, 77, 1, 28, 108, 97, 14, 53, 23, 69, 99, 31, 12, 46, 10, 7, 72, 54, 37, 24, 113, 55, 100, 85, 106, 43, 5, 3, 70, 94, 4, 8, 61, 74, 122, 68, 114, 89, 0, 80, 103, 125, 39, 42, 30, 121, 127, 76, 57, 111, 2, 118, 25};

static int test0 (
    void
) {
  const char objective[] = {"all operations should fail before initialised"};
  mybst_t * mybst = NULL;
  size_t dummy = 0;
  MY_ASSERT(1 == mybst_insert(mybst, dummy));
  MY_ASSERT(1 == mybst_finalise(&mybst));
  REPORT_SUCCESS(objective);
  return 0;
}

static int test1 (
    void
) {
  const char objective[] = {"finalisation immediately after initialisation"};
  mybst_t * mybst = NULL;
  MY_ASSERT(0 == mybst_init(&mybst));
  MY_ASSERT(0 == mybst_finalise(&mybst));
  MY_ASSERT(NULL == mybst);
  REPORT_SUCCESS(objective);
  return 0;
}

static int test2 (
    void
) {
  const char objective[] = {"normal operation"};
  mybst_t * mybst = NULL;
  MY_ASSERT(0 == mybst_init(&mybst));
  const size_t nitems = sizeof(items) / sizeof(items[0]);
  for (size_t n = 0; n < nitems; n++) {
    const size_t item = items[n];
    MY_ASSERT(0 == mybst_insert(mybst, item));
  }
  for (size_t n = 0; n < nitems; n++) {
    MY_ASSERT(0 == mybst_search(mybst, items[n]));
  }
  MY_ASSERT(1 == mybst_search(mybst, nitems));
  MY_ASSERT(1 == mybst_search(mybst, -1));
  for (size_t n = 0; n < nitems; n++) {
    MY_ASSERT(0 == mybst_delete(mybst, items[n]));
  }
  MY_ASSERT(0 == mybst_finalise(&mybst));
  MY_ASSERT(NULL == mybst);
  REPORT_SUCCESS(objective);
  return 0;
}

static int test3 (
    void
) {
  const char objective[] = {"[valgrind needed] does not leak memory"};
  const size_t nitems = sizeof(items) / sizeof(items[0]);
  mybst_t * mybst = NULL;
  // repeat twice
  for (size_t cnt = 0; cnt < 2; cnt++) {
    MY_ASSERT(0 == mybst_init(&mybst));
    for (size_t n = 0; n < nitems; n++) {
      MY_ASSERT(0 == mybst_insert(mybst, items[n]));
    }
    MY_ASSERT(0 == mybst_finalise(&mybst));
    MY_ASSERT(NULL == mybst);
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

