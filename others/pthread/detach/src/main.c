#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define NUM_THREADS 64

static void * func(
    void * arg
) {
  size_t * const thread_id = (size_t *)arg;
  usleep(rand() % 1000000); // delay up to 1 sec
  printf("Thread ID %zu\n", *thread_id);
  free(arg);
  return NULL;
}

int main(
    void
) {
  for (size_t n = 0; n < NUM_THREADS; n++) {
    pthread_t thread = NULL;
    size_t * const thread_id = malloc(sizeof(size_t));
    *thread_id = n;
    if (0 != pthread_create(&thread, NULL, func, thread_id)) {
      puts("failed to create a thread");
      return 1;
    }
    {
      const int error_code = pthread_detach(thread);
      if (0 != error_code) {
        fprintf(stderr, "pthread_detach %zu:%s\n", n, strerror(error_code));
      }
    }
  }
  // main thread waits for 5 sec
  usleep(5000000);
  return 0;
}

