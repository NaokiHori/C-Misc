#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define NUM_THREADS 8

static void * greet(
    void * argument
) {
  const size_t thread_id = *(size_t *)argument;
  printf("Hello from thread %zu\n", thread_id);
  size_t * const return_value = malloc(sizeof(size_t));
  *return_value = thread_id + 42;
  return return_value;
}

int main(
    void
) {
  pthread_t threads[NUM_THREADS] = {0};
  size_t arguments[NUM_THREADS] = {0};
  for (size_t n = 0; n < NUM_THREADS; n++) {
    arguments[n] = n;
    if (0 != pthread_create(threads + n, NULL, greet, arguments + n)) {
      puts("failed to create a thread");
      return 1;
    }
  }
  void * statuses[NUM_THREADS] = {NULL};
  for (size_t n = 0; n < NUM_THREADS; n++) {
    pthread_join(threads[n], statuses + n);
    printf("Thread %zu returned %zu\n", n, *(size_t *)statuses[n]);
    free(statuses[n]);
  }
  return 0;
}

