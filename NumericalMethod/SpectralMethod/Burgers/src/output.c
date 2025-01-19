#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <complex.h>
#include "output.h"

#define MAX_FILE_NAME_LENGTH 128

int output (
    const size_t counter,
    const int nitems,
    const size_t advective_scheme_index,
    const double * const xs,
    const double * const fs
) {
  char file_name[MAX_FILE_NAME_LENGTH] = {'\0'};
  snprintf(file_name, MAX_FILE_NAME_LENGTH, "output/scheme%zu_%010zu.dat", advective_scheme_index, counter);
  errno = 0;
  FILE * const fp = fopen(file_name, "w");
  if (NULL == fp) {
    perror(file_name);
    return 1;
  }
  for (int i = 0; i < nitems; i++) {
    fprintf(fp, "% .7e % .7e\n", xs[i], fs[i]);
  }
  fclose(fp);
  return 0;
}

