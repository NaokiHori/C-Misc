#include "./line_segment.h"

int get_line_segment(
    const vector_t * const weights,
    size_t * const nitems,
    vector_t * const intersections
) {
  // intersection with (+- 1) + y = 0
  for (size_t pm = 0; pm < 2; pm++) {
    const double w0 = 0 == pm ? + 1. : - 1.;
    const double w2 = + 1.;
    const double intersection_x = - ((*weights)[0] * w2 - (*weights)[2] * w0) / ((*weights)[1] * w2);
    const double intersection_y = -1. * w0;
    if (-1. <= intersection_x && intersection_x <= 1.) {
      intersections[*nitems][0] = 1.;
      intersections[*nitems][1] = intersection_x;
      intersections[*nitems][2] = intersection_y;
      *nitems += 1;
    }
  }
  // intersection with (+- 1) + x = 0
  for (size_t pm = 0; pm < 2; pm++) {
    const double w0 = 0 == pm ? + 1. : - 1.;
    const double w1 = + 1.;
    const double intersection_x = -1. * w0;
    const double intersection_y = - ((*weights)[0] * w1 - (*weights)[1] * w0) / ((*weights)[2] * w1);
    if (-1. <= intersection_y && intersection_y <= 1.) {
      intersections[*nitems][0] = 1.;
      intersections[*nitems][1] = intersection_x;
      intersections[*nitems][2] = intersection_y;
      *nitems += 1;
    }
  }
  if (*nitems == 2) {
    return 0;
  } else {
    return 1;
  }
}
