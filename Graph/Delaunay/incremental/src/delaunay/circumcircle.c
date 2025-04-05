#include <assert.h>
#include <float.h>
#include <math.h>
#include "circumcircle.h"

extern bool point_is_in_circumcircle(
    const point_t * const point,
    const triangle_t * const triangle
) {
  const point_t * const p0 = (*triangle)[0];
  const point_t * const p1 = (*triangle)[1];
  const point_t * const p2 = (*triangle)[2];
  const point_t v01 = {
    .x = p1->x - p0->x,
    .y = p1->y - p0->y,
  };
  const point_t v02 = {
    .x = p2->x - p0->x,
    .y = p2->y - p0->y,
  };
  const double op = 2. * (v01.x * v02.y - v01.y * v02.x);
  assert(op < - DBL_EPSILON || DBL_EPSILON < op);
  const double q1 = pow(v01.x, 2.) + pow(v01.y, 2.);
  const double q2 = pow(v02.x, 2.) + pow(v02.y, 2.);
  point_t center = {
    .x = (v02.y * q1 - v01.y * q2) / op,
    .y = (v01.x * q2 - v02.x * q1) / op,
  };
  const double radius2 = pow(center.x, 2.) + pow(center.y, 2.);
  center.x += p0->x;
  center.y += p0->y;
  const point_t d = {
    .x = point->x - center.x,
    .y = point->y - center.y,
  };
  return pow(d.x, 2.) + pow(d.y, 2.) < radius2;
}

#if defined(TEST_CIRCUMCIRCLE)

static int test1(
    void
) {
  const point_t point = {
    .x = 1.,
    .y = 1.,
  };
  const point_t vertices[] = {
    {
      .x = 0.,
      .y = 0.,
    },
    {
      .x = 1.,
      .y = 0.,
    },
    {
      .x = 0.,
      .y = 1.,
    },
  };
  const triangle_t triangle = {
    vertices + 0,
    vertices + 1,
    vertices + 2,
  };
  assert(!point_is_in_circumcircle(&point, &triangle));
  return 0;
}

static int test2(
    void
) {
  const point_t point = {
    .x = 1.,
    .y = 1.,
  };
  const point_t vertices[] = {
    {
      .x = 0.,
      .y = 0.,
    },
    {
      .x = 3.,
      .y = 0.,
    },
    {
      .x = 0.,
      .y = 3.,
    },
  };
  const triangle_t triangle = {
    vertices + 0,
    vertices + 1,
    vertices + 2,
  };
  assert(point_is_in_circumcircle(&point, &triangle));
  return 0;
}

int main(
    void
) {
  test1();
  test2();
  return 0;
}

#endif // TEST_CIRCUMCIRCLE
