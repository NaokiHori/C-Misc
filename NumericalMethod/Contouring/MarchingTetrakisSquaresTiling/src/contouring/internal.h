#if !defined(INTERNAL_H)
#define INTERNAL_H

#include "contouring.h" // contouring_contour_t, contouring_point_t

typedef struct arrow_t arrow_t;

// alias for convenience
typedef contouring_contour_t contour_t;
typedef contouring_point_t point_t;
typedef contouring_point_internal_t point_internal_t;
typedef contouring_contour_internal_t contour_internal_t;

struct contouring_point_internal_t {
  // for now thing
  char dummy;
};

struct contouring_contour_internal_t {
  // keep track of the last point of the contour to enqueue
  struct contouring_point_t * last_point;
};

#endif // INTERNAL_H
