#include <stdio.h>
#include <errno.h>
#include <assert.h>
#include "config.h"
#include "grid.h"
#include "output.h"

#define ROOT "output"
#define MAX_FILE_NAME_LENGTH 128

int output (
    const config_t * const config,
    output_config_t * const output_config,
    const char * prefix,
    const double time,
    const double * const x,
    const double * const vof
) {
  if (time < output_config->next) {
    return 0;
  }
  char file_name[MAX_FILE_NAME_LENGTH] = {'\0'};
  assert(
      snprintf(
        file_name,
        MAX_FILE_NAME_LENGTH,
        "%s/%s_%010zu.dat", ROOT, prefix, output_config->cntr
      ) < MAX_FILE_NAME_LENGTH
  );
  const size_t nitems = config->nitems;
  errno = 0;
  FILE * const fp = fopen(file_name, "w");
  if (NULL == fp) {
    perror(file_name);
    return 1;
  }
  for (size_t i = 1; i <= nitems; i++) {
    double cell_center = 0.;
    assert(0 == compute_cell_center(config, i, x, &cell_center));
    fprintf(fp, "% .15e % .15e\n", cell_center, vof[i]);
  }
  fclose(fp);
  output_config->next += output_config->freq;
  output_config->cntr += 1;
  return 0;
}

