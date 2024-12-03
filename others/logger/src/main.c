#include "logger.h"

int main(
    void
) {
  LOGGER_LOG("hello world");
  LOGGER_LOG("%s ~~(x o x)~~ %d", "hoge", 123);
  return 0;
}

