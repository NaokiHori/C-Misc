#include <errno.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <pthread.h>
#include <sys/ioctl.h>
#include <unistd.h>

volatile sig_atomic_t dummy_var = 0;

void dummy_handler (
    int signum
) {
  if (SIGWINCH == signum) {
    dummy_var = 1;
  }
}

typedef struct {
  sigset_t * sigset;
} arg_t;

void * signal_thread_workload (
    void * void_arg
) {
  arg_t * const arg = void_arg;
  sigset_t * const sigset = arg->sigset;
  while (1) {
    int signum = 0;
    const int ret = sigwait(sigset, &signum);
    if (0 == ret) {
      if (SIGWINCH == signum) {
        struct winsize win_size = {0};
        errno = 0;
        if (-1 == ioctl(STDOUT_FILENO, TIOCGWINSZ, &win_size)) {
          perror("ioctl failed");
          return NULL;
        }
        const size_t col = win_size.ws_col;
        const size_t row = win_size.ws_row;
        printf("received signal %d, (col, row) = (%zu, %zu)\n", signum, col, row);
      } else {
        printf("received unhandled signal %d\n", signum);
      }
    } else {
      errno = ret;
      perror("sigwait failed");
    }
  }
  return NULL;
}

int main (
    void
) {
  sigset_t sigset = {0};
  // block SIGWINCH signals
  {
    // NOTE: since the default behavior of SIGWINCH is SIG_IGN,
    //       it seems to be necessary to override it with a handler
    //       so that pthread_sigmask properly blocks the signal
    //       rather than discarding it immediately
    signal(SIGWINCH, dummy_handler);
    sigemptyset(&sigset);
    sigaddset(&sigset, SIGWINCH);
    const int ret = pthread_sigmask(SIG_BLOCK, &sigset, NULL);
    if (0 != ret) {
      printf("%s: pthread_sigmask failed\n", strerror(ret));
      return 1;
    }
  }
  // create a thread specialized to wait for SIGWINCH signals
  pthread_t thread_id = 0;
  {
    int ret = 0;
    // prepare an attribute to make the thread detached
    pthread_attr_t thread_attr = {0};
    ret = pthread_attr_init(&thread_attr);
    if (0 != ret) {
      printf("%s: pthread_attr_init failed\n", strerror(ret));
      return 1;
    }
    ret = pthread_attr_setdetachstate(&thread_attr, PTHREAD_CREATE_DETACHED);
    if (0 != ret) {
      printf("%s: pthread_attr_setdetachstate failed\n", strerror(ret));
      return 1;
    }
    arg_t arg = {
      .sigset = &sigset,
    };
    ret = pthread_create(&thread_id, &thread_attr, signal_thread_workload, &arg);
    if (0 != ret) {
      printf("%s: pthread_create failed\n", strerror(ret));
      return 1;
    }
    ret = pthread_attr_destroy(&thread_attr);
    if (0 != ret) {
      printf("%s: pthread_attr_destroy failed\n", strerror(ret));
      return 1;
    }
  }
  while (1) {
    puts("waiting for user input ('q' to abort)...");
    fflush(stdout);
#define BUFFER_SIZE 64
    char buffer[BUFFER_SIZE] = {"\0"};
    const ssize_t bytes_read = read(STDIN_FILENO, buffer, (BUFFER_SIZE - 1) * sizeof(char));
    if (0 < bytes_read) {
      buffer[bytes_read] = '\0';
      printf("user Input: %s", buffer);
      if (0 == strcmp(buffer, "q\n")) {
        pthread_cancel(thread_id);
        break;
      }
    }
  }
  return 0;
}

