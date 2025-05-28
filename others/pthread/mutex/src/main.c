#include <pthread.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#define NUM_THREADS 64
static const size_t num_increments = 1000;

typedef struct {
  size_t counter;
  pthread_mutex_t mutex;
} argument_t;

static void * increment(
    void * void_argument
) {
  argument_t * const argument = (argument_t *)void_argument;
  size_t * const counter = &argument->counter;
  pthread_mutex_t * const mutex = &argument->mutex;
  for (size_t n = 0; n < num_increments; n++) {
    pthread_mutex_lock(mutex);
    *counter += 1;
    pthread_mutex_unlock(mutex);
    usleep(1); // 1 microsecond delay
  }
  return NULL;
}

int main(
    void
) {
  pthread_t threads[NUM_THREADS] = {0};
  argument_t argument = {
    .counter = 0,
    .mutex = {0},
  };
  {
    const int error_code = pthread_mutex_init(&argument.mutex, NULL);
    if (0 != error_code) {
      fprintf(stderr, "Failed pthread_mutex_init: %s\n", strerror(error_code));
      return 1;
    }
  }
  for (size_t n = 0; n < NUM_THREADS; n++) {
    if (0 != pthread_create(threads + n, NULL, increment, &argument)) {
      puts("failed to create a thread");
      return 1;
    }
  }
  for (size_t n = 0; n < NUM_THREADS; n++) {
    pthread_join(threads[n], NULL);
  }
  {
    const int error_code = pthread_mutex_destroy(&argument.mutex);
    if (0 != error_code) {
      fprintf(stderr, "Failed pthread_mutex_destroy: %s\n", strerror(error_code));
      return 1;
    }
  }
  printf("Counter: %zu, Expected: %zu\n", argument.counter, NUM_THREADS * num_increments);
  return 0;
}

