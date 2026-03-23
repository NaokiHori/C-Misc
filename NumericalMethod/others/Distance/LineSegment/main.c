#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define NDIMS 2

typedef double vector_t[NDIMS];

static double dot (
    const vector_t * const a,
    const vector_t * const b
) {
  double result = 0.;
  for (size_t n = 0; n < NDIMS; n++) {
    result += (*a)[n] * (*b)[n];
  }
  return result;
}

static double find_squared_distance (
    const vector_t line_segment[],
    const vector_t * const p
) {
  // let
  //   line segment: AB
  //   point: P
  //   line perpendicular to AB and on P
  //   intersection: Q
  // vec(q)
  //   =
  //   vec(a) + t * (vec(b) - vec(a))
  //   =
  //   vec(p) + s * (vec(q) - vec(p))
  // since vec(b) - vec(a) and vec(q) - vec(p) are perpendicular, we have
  //   vec(a) dot (vec(b) - vec(a))
  //   +
  //   t * (vec(b) - vec(a)) dot (vec(b) - vec(a))
  //   =
  //   vec(p) dot (vec(b) - vec(a))
  // and
  //   vec(a) dot (vec(q) - vec(p))
  //   +
  //   t * (vec(b) - vec(a)) dot (vec(b) - vec(a))
  //   =
  //   vec(p) dot (vec(b) - vec(a)),
  // from which t, s are found, respectively
  const vector_t * const a = line_segment + 0;
  const vector_t * const b = line_segment + 1;
  const vector_t ba = {
    (*b)[0] - (*a)[0],
    (*b)[1] - (*a)[1],
  };
  const vector_t pa = {
    (*p)[0] - (*a)[0],
    (*p)[1] - (*a)[1],
  };
  const vector_t pb = {
    (*p)[0] - (*b)[0],
    (*p)[1] - (*b)[1],
  };
  const double t = dot(&pa, &ba) / dot(&ba, &ba);
  if (t < 0.) {
    // the nearest point is A
    return dot(&pa, &pa);
  } else if (1. < t) {
    // the nearest point is B
    return dot(&pb, &pb);
  } else {
    // find distance between P and Q
    const vector_t q = {
      (*a)[0] + t * ba[0],
      (*a)[1] + t * ba[1],
    };
    const vector_t pq = {
      q[0] - (*p)[0],
      q[1] - (*p)[1],
    };
    return dot(&pq, &pq);
  }
}

static int output (
    const char file_name[],
    const size_t nitems,
    const double * const items
) {
  FILE * const fp = fopen(file_name, "w");
  if (NULL == fp) {
    perror(file_name);
    return 1;
  }
  for (size_t n = 0; n < nitems; n++) {
    fprintf(fp, "% .7e\n", items[n]);
  }
  fclose(fp);
  return 0;
}

int main (
    void
) {
  srand(0);
  const vector_t line_segment[2] = {
    {- 1. * rand() / RAND_MAX, - 1. * rand() / RAND_MAX},
    {+ 1. * rand() / RAND_MAX, + 1. * rand() / RAND_MAX},
  };
  {
    const char file_name[] = "line.dat";
    FILE * const fp = fopen(file_name, "w");
    if (NULL == fp) {
      perror(file_name);
      return 1;
    }
    fprintf(fp, "% .7e % .7e\n", line_segment[0][0], line_segment[0][1]);
    fprintf(fp, "% .7e % .7e\n", line_segment[1][0], line_segment[1][1]);
    fclose(fp);
  }
  const size_t nitems = 64;
  double * xs = malloc(nitems * sizeof(double));
  double * ys = malloc(nitems * sizeof(double));
  double * dists = malloc(nitems * nitems * sizeof(double));
  for (size_t n = 0; n < nitems; n++) {
    xs[n] = - 1. + 2. / (nitems - 1) * n;
    ys[n] = - 1. + 2. / (nitems - 1) * n;
  }
  for (size_t j = 0; j < nitems; j++) {
    for (size_t i = 0; i < nitems; i++) {
      const vector_t point = {xs[i], ys[j]};
      dists[j * nitems + i] = sqrt(find_squared_distance(line_segment, &point));
    }
  }
  output("xs.dat", nitems, xs);
  output("ys.dat", nitems, ys);
  output("zs.dat", nitems * nitems, dists);
  free(xs);
  free(ys);
  free(dists);
  return 0;
}

