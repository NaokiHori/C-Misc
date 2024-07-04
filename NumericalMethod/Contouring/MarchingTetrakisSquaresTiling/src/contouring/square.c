#include "./square.h"

double get_centered_value (
    const double vertex_values[NUM_SQUARE_VERTICES]
) {
  return 0.25 * (
      + vertex_values[0]
      + vertex_values[1]
      + vertex_values[2]
      + vertex_values[3]
  );
}

