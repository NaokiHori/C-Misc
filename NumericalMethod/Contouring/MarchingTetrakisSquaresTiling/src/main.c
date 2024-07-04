#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <math.h>
#include "contouring.h"

static int construct_dataset (
    const size_t nx,
    const size_t ny,
    double * const xs,
    double * const ys,
    double * const field
) {
  if (nx < 2 || ny < 2) {
    printf("at least two points are needed for each direction: (%zu, %zu)\n", nx, ny);
    return 1;
  }
  const double lx = 1.;
  const double ly = 1.;
  for (size_t i = 0; i < nx; i++) {
    xs[i] = lx * i / (nx - 1);
  }
  for (size_t j = 0; j < ny; j++) {
    ys[j] = ly * j / (ny - 1);
  }
  const double pi = 3.14159265358932384626;
  // superpose sinusoidal waves to create a random field
  const size_t nfreqx = 12;
  const size_t nfreqy = 12;
  for (size_t ky = 1; ky < nfreqy; ky++) {
    const double yamp = - 0.5 + 1. * rand() / RAND_MAX;
    const double yphase = 2. * pi * rand() / RAND_MAX;
    for (size_t kx = 1; kx < nfreqx; kx++) {
      const double xamp = - 0.5 + 1. * rand() / RAND_MAX;
      const double xphase = 2. * pi * rand() / RAND_MAX;
      for (size_t j = 0; j < ny; j++) {
        const double y = ys[j];
        for (size_t i = 0; i < nx; i++) {
          const double x = xs[i];
          field[j * nx + i] += 1.
            * xamp * sin(2. * pi / lx * kx * x + xphase)
            * yamp * sin(2. * pi / ly * ky * y + yphase);
        }
      }
    }
  }
  // enforce zero mean
  double sum = 0.;
  for (size_t n = 0; n < nx * ny; n++) {
    sum += field[n];
  }
  sum /= 1. * nx * ny;
  for (size_t n = 0; n < nx * ny; n++) {
    field[n] -= sum;
  }
  return 0;
}

static int check (
    const contouring_contour_t * contour
) {
  const char file_name[] = {"contours-gnuplot.dat"};
  errno = 0;
  FILE * fp = fopen(file_name, "w");
  if (NULL == fp) {
    perror(file_name);
    return 1;
  }
  while (contour) {
    for (const contouring_point_t * point = contour->point; point; point = point->next) {
      fprintf(fp, "% .15e % .15e\n", point->x, point->y);
    }
    fprintf(fp, "\n");
    contour = contour->next;
  }
  fclose(fp);
  return 0;
}

int main (
    void
) {
  int retval = 0;
  // prepare
  const double threshold = 0.;
  const size_t sizes[] = {129, 129};
  double * const grids[] = {
    malloc(sizes[0] * sizeof(double)),
    malloc(sizes[1] * sizeof(double)),
  };
  double * const field = calloc(sizes[0] * sizes[1], sizeof(double));
  contouring_contour_t * contour = NULL;
  if (0 != construct_dataset(sizes[0], sizes[1], grids[0], grids[1], field)) {
    puts("failed to prepare dataset");
    retval = 1;
    goto abort;
  }
  // extract contours
  if (0 != contouring_exec(threshold, sizes, grids, field, &contour)) {
    puts("failed to extract contours");
    retval = 1;
    goto abort;
  }
  check(contour);
abort:
  // clean-up
  contouring_cleanup(&contour);
  free(grids[0]);
  free(grids[1]);
  free(field);
  return retval;
}

