#if !defined(LOGGER_H)
#define LOGGER_H

#include <stdio.h>

#define LOGGER_LOG(...) \
  do { \
    fprintf(stderr, "[%s:%d] ", __FILE__, __LINE__); \
    fprintf(stderr, __VA_ARGS__); \
    fprintf(stderr, "\n"); \
  } while(0)

#endif // LOGGER_H
