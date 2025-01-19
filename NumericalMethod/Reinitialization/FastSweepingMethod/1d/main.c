#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <stdbool.h>

static int impose_bc(
    const size_t nitems,
    double * const sdf
) {
  // periodic boundary condition
  sdf[0] = sdf[nitems];
  sdf[nitems + 1] = sdf[1];
  return 0;
}

static double min(
    const double a,
    const double b
) {
  return a < b ? a : b;
}

static double max(
    const double a,
    const double b
) {
  return a < b ? b : a;
}

static double compute_new_value(
    const double grid_size,
    const double sdfs[3]
) {
  if (sdfs[1] < 0.) {
    return max(max(sdfs[0], sdfs[2]) - grid_size, sdfs[1]);
  } else {
    return min(min(sdfs[0], sdfs[2]) + grid_size, sdfs[1]);
  }
}

static int forward_sweep(
    const size_t nitems,
    const double grid_size,
    const bool * const are_fixed,
    double * const sdf
) {
  for (size_t i = 1; i <= nitems; i++) {
    if (are_fixed[i]) {
      continue;
    }
    sdf[i] = compute_new_value(
        grid_size,
        (double [3]){sdf[i - 1], sdf[i], sdf[i + 1]}
    );
  }
  impose_bc(nitems, sdf);
  return 0;
}

static int backward_sweep(
    const size_t nitems,
    const double grid_size,
    const bool * const are_fixed,
    double * const sdf
) {
  for (size_t i = nitems; 0 < i; i--) {
    if (are_fixed[i]) {
      continue;
    }
    sdf[i] = compute_new_value(
        grid_size,
        (double [3]){sdf[i - 1], sdf[i], sdf[i + 1]}
    );
  }
  impose_bc(nitems, sdf);
  return 0;
}

static int output(
    const char file_name[],
    const size_t nitems,
    const double * const x,
    const double * const sdf
) {
  FILE * const fp = fopen(file_name, "w");
  if (NULL == fp) {
    puts("Failed to open file");
    return 1;
  }
  for (size_t i = 0; i < nitems; i++) {
    fprintf(fp, "% .7e % .7e\n", x[i], sdf[i]);
  }
  fclose(fp);
  return 0;
}

int main(
    void
) {
  const double length = 1.;
  const size_t nitems = 128;
  const double grid_size = length / nitems;
  double * const x = malloc(nitems * sizeof(double));
  for (size_t i = 0; i < nitems; i++) {
    x[i] = 0.5 * (2 * i + 1) * grid_size;
  }
  // give two points whose signed distances are known and pinned
#define N_FIXED_POINTS 2
  const size_t fixed_points[N_FIXED_POINTS] = {nitems / 8, nitems / 4};
  const double fixed_values[N_FIXED_POINTS] = {-0.3 * grid_size, 0.2 * grid_size};
  double * const sdf = malloc((nitems + 2) * sizeof(double));
  bool * const are_fixed = malloc((nitems + 2) * sizeof(bool));
  // give internal / external information
  //   as the initial signed-distance function
  const double large = 2. * length;
  for (size_t i = 1; i <= nitems; i++) {
    if (i < fixed_points[0]) {
      sdf[i] = - large;
    } else if (i == fixed_points[0]) {
      sdf[i] = fixed_values[0];
    } else if (i < fixed_points[1]) {
      sdf[i] = + large;
    } else if (i == fixed_points[1]) {
      sdf[i] = fixed_values[1];
    } else {
      sdf[i] = - large;
    }
  }
  impose_bc(nitems, sdf);
  output("before.dat", nitems, x, sdf + 1);
  for (size_t i = 0; i < nitems + 2; i++) {
    are_fixed[i] = false;
    if (i == fixed_points[0] || i == fixed_points[1]) {
      are_fixed[i] = true;
    }
  }
  // forward sweeps for twice to accommodate periodicity
  forward_sweep(nitems, grid_size, are_fixed, sdf);
  forward_sweep(nitems, grid_size, are_fixed, sdf);
  // backward sweeps for twice to accommodate periodicity
  backward_sweep(nitems, grid_size, are_fixed, sdf);
  backward_sweep(nitems, grid_size, are_fixed, sdf);
  // output resulting signed-distance field
  output("after.dat", nitems, x, sdf + 1);
  // clean-up
  free(x);
  free(sdf);
  free(are_fixed);
  return 0;
}
