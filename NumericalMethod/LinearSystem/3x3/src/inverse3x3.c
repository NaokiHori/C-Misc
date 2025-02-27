#include <float.h>
#include <math.h>
#include <stdio.h>
#include "inverse3x3.h"

int find_inverse_matrix(
    double * const a
) {
  const double a00 = a[0 * 3 + 0];
  const double a01 = a[0 * 3 + 1];
  const double a02 = a[0 * 3 + 2];
  const double a10 = a[1 * 3 + 0];
  const double a11 = a[1 * 3 + 1];
  const double a12 = a[1 * 3 + 2];
  const double a20 = a[2 * 3 + 0];
  const double a21 = a[2 * 3 + 1];
  const double a22 = a[2 * 3 + 2];
  const double determinant =
    + a00 * a11 * a22
    + a01 * a12 * a20
    + a02 * a10 * a21
    - a02 * a11 * a20
    - a01 * a10 * a22
    - a00 * a12 * a21;
  if (fabs(determinant) < DBL_EPSILON) {
    printf("singular matrix (determinant: % .1e)\n", determinant);
    return 1;
  }
  const double detinv = 1. / determinant;
  a[0 * 3 + 0] = detinv * ( + a11 * a22 - a12 * a21 );
  a[0 * 3 + 1] = detinv * ( - a01 * a22 + a02 * a21 );
  a[0 * 3 + 2] = detinv * ( + a01 * a12 - a02 * a11 );
  a[1 * 3 + 0] = detinv * ( - a10 * a22 + a12 * a20 );
  a[1 * 3 + 1] = detinv * ( + a00 * a22 - a02 * a20 );
  a[1 * 3 + 2] = detinv * ( - a00 * a12 + a02 * a10 );
  a[2 * 3 + 0] = detinv * ( + a10 * a21 - a11 * a20 );
  a[2 * 3 + 1] = detinv * ( - a00 * a21 + a01 * a20 );
  a[2 * 3 + 2] = detinv * ( + a00 * a11 - a01 * a10 );
  return 0;
}

