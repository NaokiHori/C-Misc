#include <float.h>
#include <math.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

#define NDIMS 2
#define PI 3.14159265358979324

typedef double vec_t[NDIMS];

typedef struct {
  const vec_t center;
  const double radius;
  // from 0 to 1, counter-clockwise direction
  const double angles[2];
} arc_t;

static double compute_l2(
  const double x,
  const double y
) {
  return sqrt(
    + pow(x, 2.)
    + pow(y, 2.)
  );
}

static int compute_distance(
  const arc_t * const arc,
  const vec_t * const point,
  double * const distance
) {
  const double (* angles)[2] = &arc->angles;
  const bool is_reversed = (*angles)[1] < (*angles)[0];
  if ((*angles)[0] < - PI || PI <= (*angles)[0]) {
    return 1;
  }
  if ((*angles)[1] < - PI || PI <= (*angles)[1]) {
    return 1;
  }
  // from center to point
  const vec_t vec_cp = {
    (*point)[0] - arc->center[0],
    (*point)[1] - arc->center[1],
  };
  const double vec_cp_atan2 = atan2(vec_cp[1], vec_cp[0]);
  *distance = DBL_MAX;
  if (
    (is_reversed && !((*angles)[1] < vec_cp_atan2 && vec_cp_atan2 < (*angles)[0]))
    ||
    (!is_reversed && (*angles)[0] < vec_cp_atan2 && vec_cp_atan2 < (*angles)[1])
  ) {
    *distance = fabs(
      arc->radius
      -
      compute_l2(
        vec_cp[0],
        vec_cp[1]
      )
    );
  } else {
    *distance = fmin(
      *distance,
      compute_l2(
        arc->center[0] + arc->radius * cos((*angles)[0]) - (*point)[0],
        arc->center[1] + arc->radius * sin((*angles)[0]) - (*point)[1]
      )
    );
    *distance = fmin(
      *distance,
      compute_l2(
        arc->center[0] + arc->radius * cos((*angles)[1]) - (*point)[0],
        arc->center[1] + arc->radius * sin((*angles)[1]) - (*point)[1]
      )
    );
  }
  return 0;
}

static int output_arc(
  const char file_name[],
  const arc_t * const arc
) {
  FILE * const fp = fopen(file_name, "w");
  if (NULL == fp) {
    return 1;
  }
  fprintf(fp, "{\n");
  fprintf(fp, "  \"center\": {\n");
  fprintf(fp, "    \"x\": % .7e,\n", arc->center[0]);
  fprintf(fp, "    \"y\": % .7e\n", arc->center[1]);
  fprintf(fp, "  },\n");
  fprintf(fp, "  \"radius\": % .7e,\n", arc->radius);
  fprintf(fp, "  \"angles\": [\n");
  fprintf(fp, "    % .7e,\n", arc->angles[0]);
  fprintf(fp, "    % .7e\n", arc->angles[1]);
  fprintf(fp, "  ]\n");
  fprintf(fp, "}\n");
  fclose(fp);
  return 0;
}

static int output_dat(
  const char file_name[],
  const size_t n_points,
  const double * const points
) {
  FILE * const fp = fopen(file_name, "w");
  if (NULL == fp) {
    return 1;
  }
  for (size_t n = 0; n < n_points; n++) {
    fprintf(fp, "% .7e\n", points[n]);
  }
  fclose(fp);
  return 0;
}

int main(
  void
) {
  const arc_t arc = {
    .center = {
      0.1,
      0.2,
    },
    .radius = 0.5,
    .angles = {
      3. * PI / 4.,
      1. * PI / 6.,
    },
  };
  const size_t n_points[NDIMS] = {64, 64};
  double * const x_points = malloc(n_points[0] * sizeof(double));
  double * const y_points = malloc(n_points[1] * sizeof(double));
  double * const distances = malloc(n_points[0] * n_points[1] * sizeof(double));
  for (size_t i = 0; i < n_points[0]; i++) {
    x_points[i] = - 1. + 2. * i / (n_points[0] - 1);
  }
  for (size_t j = 0; j < n_points[1]; j++) {
    y_points[j] = - 1. + 2. * j / (n_points[1] - 1);
  }
  for (size_t j = 0; j < n_points[1]; j++) {
    for (size_t i = 0; i < n_points[0]; i++) {
      const vec_t point = {
        x_points[i],
        y_points[j],
      };
      double * const distance = distances + j * n_points[0] + i;
      if (0 != compute_distance(&arc, &point, distance)) {
        return 1;
      }
    }
  }
  output_dat("x_points.dat", n_points[0], x_points);
  output_dat("y_points.dat", n_points[1], y_points);
  output_dat("distances.dat", n_points[0] * n_points[1], distances);
  output_arc("arc.json", &arc);
  free(x_points);
  free(y_points);
  free(distances);
  return 0;
}
