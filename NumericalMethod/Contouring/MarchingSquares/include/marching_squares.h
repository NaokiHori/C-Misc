#if !defined(MARCHING_SQUARES_H)
#define MARCHING_SQUARES_H

#include <stddef.h> // size_t

// datatype to store a point
typedef struct marching_squares_point_t {
  // coordinate of a point
  double x;
  double y;
  // pointer to the next point
  // NULL if this is the last item
  struct marching_squares_point_t * next;
} marching_squares_point_t;

// datatype to store a contour
typedef struct marching_squares_contour_t {
  // pointer to the first point of the contour
  marching_squares_point_t * point;
  // pointer to the next contour
  // NULL if this is the last item
  struct marching_squares_contour_t * next;
} marching_squares_contour_t;

extern int marching_squares_exec (
    const double threshold,
    const size_t * const sizes,
    double * const * const grids,
    const double * const field,
    marching_squares_contour_t ** const contour
);

extern int marching_squares_cleanup (
    marching_squares_contour_t ** const contour
);

#endif // MARCHING_SQUARES_H
