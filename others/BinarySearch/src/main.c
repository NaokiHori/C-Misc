#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <limits.h>
#include "binary_search.h"

static int compare_int (
    const void * p_void_a,
    const void * p_void_b
) {
  const int * const p_int_a = (int *)p_void_a;
  const int * const p_int_b = (int *)p_void_b;
  const int a = *p_int_a;
  const int b = *p_int_b;
  if (a < b) {
    return - 1;
  } else if (b < a) {
    return + 1;
  } else {
    return 0;
  }
}

static int case_int (
    void
) {
  int items[] = {2, 3, 5, 8, 13};
  const int nitems = sizeof(items) / sizeof(items[0]);
  qsort(items, nitems, sizeof(int), compare_int);
  puts("input items:");
  for (int n = 0; n < nitems; n++) {
    printf("%3d: %3d\n", n, items[n]);
  }
  puts("comparison:");
  const int min = items[         0] - 1;
  const int max = items[nitems - 1] + 1;
  for (int target = min; target <= max; target++) {
    int index = -1;
    if (0 != binary_search(items, nitems, sizeof(int), compare_int, &target, &index)) {
      printf("binary search failed: %d\n", target);
      return 1;
    }
    if (-1 == index) {
      assert(target < items[0]);
    } else if (nitems - 1 == index) {
      assert(items[nitems - 1] <= target);
    } else {
      assert(items[index] <= target);
      assert(target < items[index + 1]);
    }
    const int bef =         -1 == index ? INT_MIN : items[index    ];
    const int aft = nitems - 1 == index ? INT_MAX : items[index + 1];
    printf("%12d <= %12d < %12d\n", bef, target, aft);
  }
  return 0;
}

static int compare_str (
    const void * p_void_a,
    const void * p_void_b
) {
  char ** const pp_str_a = (char **)p_void_a;
  char ** const pp_str_b = (char **)p_void_b;
  const char * const str_a = *pp_str_a;
  const char * const str_b = *pp_str_b;
  const size_t len_a = strlen(str_a);
  const size_t len_b = strlen(str_b);
  for (size_t n = 0; n < (len_a < len_b ? len_b : len_a); n++) {
    const char c_a = len_a - 1 < n ? 'a' - 1 : str_a[n];
    const char c_b = len_b - 1 < n ? 'a' - 1 : str_b[n];
    if (c_a < c_b) {
      return - 1;
    } else if (c_b < c_a) {
      return + 1;
    }
  }
  return 0;
}

static int case_str (
    void
) {
  char * items[] = {
    "lorem",
    "ipsum",
    "dolor",
    "sit",
    "amet",
    "consectetur",
    "adipiscing",
    "elit",
    "sed",
    "do",
    "eiusmod",
    "tempor",
    "incididunt",
    "ut",
    "labore",
    "et",
    "dolore",
    "magna",
    "aliqua",
  };
  const int nitems = sizeof(items) / sizeof(items[0]);
  qsort(items, nitems, sizeof(char *), compare_str);
  puts("input items:");
  for (int n = 0; n < nitems; n++) {
    printf("%3d: %12s\n", n, items[n]);
  }
  const char * targets[] = {
    "ut",
    "enim",
    "ad",
    "minim",
    "veniam",
    "quis",
    "nostrud",
    "exercitation",
    "ullamco",
    "laboris",
    "nisi",
    "ut",
    "aliquip",
    "ex",
    "ea",
    "commodo",
    "consequat",
  };
  const int target_nitems = sizeof(targets) / sizeof(targets[0]);
  puts("comparison:");
  for (int n = 0; n < target_nitems; n++) {
    const char * const target = targets[n];
    int index = -1;
    if (0 != binary_search(items, nitems, sizeof(char *), compare_str, &target, &index)) {
      printf("binary search failed: %s\n", target);
      return 1;
    }
    const char * bef =         -1 == index ? "<N/A>" : items[index    ];
    const char * aft = nitems - 1 == index ? "<N/A>" : items[index + 1];
    printf("%12s <= %12s < %12s\n", bef, target, aft);
  }
  return 0;
}

int main (
    void
) {
  case_int();
  case_str();
  return 0;
}

