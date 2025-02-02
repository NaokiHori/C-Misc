#if !defined(CONTOURING_H)
#define CONTOURING_H

#include <stddef.h> // size_t
#include <stdbool.h> // bool

typedef struct contouring_point_internal_t contouring_point_internal_t;
typedef struct contouring_contour_internal_t contouring_contour_internal_t;

// datatype to store a point
typedef struct contouring_point_t {
  // coordinate of a point
  double x;
  double y;
  // pointer to the next point
  // NULL if this is the last item
  struct contouring_point_t * next;
  // for internal use
  contouring_point_internal_t * internal;
} contouring_point_t;

// datatype to store a contour
typedef struct contouring_contour_t {
  // if this is a closed contour
  bool is_closed;
  // number of points consisting this contour
  size_t npoints;
  // pointer to the first point of the contour
  contouring_point_t * point;
  // pointer to the next contour
  // NULL if this is the last item
  struct contouring_contour_t * next;
  // for internal use
  contouring_contour_internal_t * internal;
} contouring_contour_t;

extern int contouring_exec(
    const double threshold,
    const size_t * const sizes,
    double * const * const grids,
    const double * const field,
    contouring_contour_t ** const contour
);

extern int contouring_cleanup(
    contouring_contour_t ** const contour
);

#endif // CONTOURING_H
