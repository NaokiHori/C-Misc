#include <stdio.h>
#include <stdlib.h>
#include "interp_cubic.h"

// Cubic spline interpolation with edge treatment.
// Given four points
//   gx[0 : 3]
//   gy[0 : 3]
//   we consider to interpolate between gx[1] and gx[2] using a cubic function
//   f(x) = c3 x^3 + c2 x^2 + c1 x^1 + c0 x^0.
// First the global coordinate system is mapped to a local coordinate system,
//   such that lx[1] = -1 and lx[2] = +1 are satisfied.
// At x = -1 and x = +1, I request to satisfy
//   f(-1)  = y[1]
//   f(+1)  = y[2]
//   f'(-1) = dydx[1]
//   f'(+1) = dydx[2],
//   where dydxs are approximated by the finite-difference scheme, namely
//   dydx[1] = (y[2] - y[0]) / (x[2] - x[0])
//   dydx[2] = (y[3] - y[1]) / (x[3] - x[1])
// These four conditions uniquely determine c0, c1, c2, c3.
//
// At the edges, only one derivative is requested
//   and a quadratic function is fitted.

// a piecewise function
typedef struct {
  // two denominators of dy/dx to compute derivatives
  double dxs[2];
  // coefficients of a cubic function,
  //   which are updated using the neighbouring y values
  double coefs[4];
} pfunc_t;

// point to be interpolated
typedef struct {
  // position in a local coordinate system
  double x;
  // interpolating piecewise function
  const pfunc_t * pfunc;
} point_t;

struct interp_cubic_plan_t {
  size_t n1;
  size_t n2;
  pfunc_t * pfuncs;
  point_t * points;
};

static void * memory_alloc (
    const size_t nitems,
    const size_t size
) {
  void * ptr = malloc(nitems * size);
  if (NULL == ptr) {
    fprintf(stderr, "FATAL: memory allocation error\n");
    exit(EXIT_FAILURE);
  }
  return ptr;
}

static void memory_free (
    void * ptr
) {
  free(ptr);
}

// transform from global to local coordinate systems
static inline double transform (
    const double gxm,
    const double gxp,
    const double gx
) {
  // gx in [gxm : gxp]
  // ->
  // x in [-1 : +1]
  return - 1. + 2. / (gxp - gxm) * (gx - gxm);
}

// fit a piecewise cubic (quadratic) polynomial
//   for each interval (xs[i], xs[i+1]), where i = [0 : n - 1]
// since the function values are unknown at this point,
//   anyway compute "dx"s
// global coordinates are first transformed to a local coordinate system first
// --x----x------x-------x--
//  x0   x1     x2      x3
static int init_pfuncs (
    const size_t n,
    const double * xs,
    pfunc_t * pfuncs
) {
  // left-bound (x0 is missing), fit a quadratic function
  {
    pfunc_t * const pfunc = pfuncs + 0;
    const double x1 = transform(xs[0], xs[1], xs[0]);
    const double x3 = transform(xs[0], xs[1], xs[2]);
    pfunc->dxs[0] = + x3 - x1;
    pfunc->dxs[1] = + x3 - x1;
  }
  // center, fit a cubic function
  for (size_t i = 1; i < n - 2; i++) {
    pfunc_t * const pfunc = pfuncs + i;
    const double x0 = transform(xs[i    ], xs[i + 1], xs[i - 1]);
    const double x1 = transform(xs[i    ], xs[i + 1], xs[i    ]);
    const double x2 = transform(xs[i    ], xs[i + 1], xs[i + 1]);
    const double x3 = transform(xs[i    ], xs[i + 1], xs[i + 2]);
    pfunc->dxs[0] = + x2 - x0;
    pfunc->dxs[1] = + x3 - x1;
  }
  // right-bound (x3 is missing), fit a quadratic function
  {
    pfunc_t * const pfunc = pfuncs + n - 2;
    const double x0 = transform(xs[n - 2], xs[n - 1], xs[n - 3]);
    const double x2 = transform(xs[n - 2], xs[n - 1], xs[n - 1]);
    pfunc->dxs[0] = + x2 - x0;
    pfunc->dxs[1] = + x2 - x0;
  }
  return 0;
}

// determine mapping between x1 and x2
static int init_points (
    const size_t n1,
    const size_t n2,
    const double * gx1s,
    const double * gx2s,
    const pfunc_t * pfuncs,
    point_t * points
) {
  // NOTE: O(N^2)
  for (size_t i2 = 0; i2 < n2; i2++) {
    point_t * point = points + i2;
    const double gx2 = gx2s[i2];
    size_t index = n1 - 2;
    for (size_t i1 = 1; i1 < n1 - 1; i1++) {
      const double gx1 = gx1s[i1];
      if (gx2 < gx1) {
        index = i1 - 1;
        break;
      }
    }
    point->pfunc = pfuncs + index;
    point->x = transform(gx1s[index], gx1s[index + 1], gx2);
  }
  return 0;
}

// fit a cubic (or quadratic, at boundaries) function for each interval
static int fit_pfuncs (
    const size_t n,
    const double * y1s,
    pfunc_t * pfuncs
) {
  for (size_t i = 0; i < n - 1; i++) {
    pfunc_t * const pfunc = pfuncs + i;
    const double * const dxs = pfunc->dxs;
    double * coefs = pfunc->coefs;
    if (0 == i) {
      // left-bound, quadratic function
      const double * y1s_ = y1s;
      const double y1 = y1s_[0];
      const double y2 = y1s_[1];
      const double dydx2 = (y1s_[2] - y1s_[0]) / dxs[1];
      coefs[0] = 1. / 4. * (+ 1. * y1 + 3. * y2 - 2. * dydx2);
      coefs[1] = 1. / 4. * (- 2. * y1 + 2. * y2             );
      coefs[2] = 1. / 4. * (+ 1. * y1 - 1. * y2 + 2. * dydx2);
      coefs[3] = 0.;
    } else if (n - 2 == i) {
      // right-bound, quadratic function
      const double * y1s_ = y1s + i - 1;
      const double y1 = y1s_[1];
      const double y2 = y1s_[2];
      const double dydx1 = (y1s_[2] - y1s_[0]) / dxs[0];
      coefs[0] = 1. / 4. * (+ 3. * y1 + 1. * y2 + 2. * dydx1);
      coefs[1] = 1. / 4. * (- 2. * y1 + 2. * y2             );
      coefs[2] = 1. / 4. * (- 1. * y1 + 1. * y2 - 2. * dydx1);
      coefs[3] = 0.;
    } else {
      // center, cubic function
      const double * y1s_ = y1s + i - 1;
      const double y1 = y1s_[1];
      const double y2 = y1s_[2];
      const double dydx1 = (y1s_[2] - y1s_[0]) / dxs[0];
      const double dydx2 = (y1s_[3] - y1s_[1]) / dxs[1];
      coefs[0] = 1. / 4. * (+ 2. * y1 + 2. * y2 + 1. * dydx1 - 1. * dydx2);
      coefs[1] = 1. / 4. * (- 3. * y1 + 3. * y2 - 1. * dydx1 - 1. * dydx2);
      coefs[2] = 1. / 4. * (                    - 1. * dydx1 + 1. * dydx2);
      coefs[3] = 1. / 4. * (+ 1. * y1 - 1. * y2 + 1. * dydx1 + 1. * dydx2);
    }
  }
  return 0;
}

static int interpolate (
    const size_t n,
    const point_t * points,
    double * ys
) {
  for (size_t i = 0; i < n; i++) {
    const point_t * point = points + i;
    const double x = point->x;
    const pfunc_t * pfunc = point->pfunc;
    const double * coefs = pfunc->coefs;
    const double x0 = 1.;
    const double x1 = x0 * x;
    const double x2 = x1 * x;
    const double x3 = x2 * x;
    ys[i] =
      + coefs[0] * x0
      + coefs[1] * x1
      + coefs[2] * x2
      + coefs[3] * x3;
  }
  return 0;
}

int interp_cubic_initialise (
    const size_t n1,
    const size_t n2,
    const double * x1s,
    const double * x2s,
    interp_cubic_plan_t ** plan
) {
  const size_t n_minimum = 3;
  if (n1 < n_minimum) {
    printf("n1 (%zu) should be equal or larger than %zu\n", n1, n_minimum);
    return 1;
  }
  if (n2 < n_minimum) {
    printf("n2 (%zu) should be equal or larger than %zu\n", n2, n_minimum);
    return 1;
  }
  *plan = memory_alloc(1, sizeof(interp_cubic_plan_t));
  (*plan)->n1 = n1;
  (*plan)->n2 = n2;
  pfunc_t ** pfuncs = &(*plan)->pfuncs;
  point_t ** points = &(*plan)->points;
  // piecewise functions for each interval of x1s
  *pfuncs = memory_alloc(n1 - 1, sizeof(pfunc_t));
  *points = memory_alloc(n2,     sizeof(point_t));
  // initialise piecewise cubic (quadratic) polynomial
  if (0 != init_pfuncs(n1, x1s, *pfuncs)) {
    goto abort;
  }
  // initialise each point in x2s
  if (0 != init_points(n1, n2, x1s, x2s, *pfuncs, *points)) {
    goto abort;
  }
  return 0;
abort:
  memory_free(*pfuncs);
  memory_free(*points);
  memory_free(*plan);
  return 1;
}

int interp_cubic_execute (
    interp_cubic_plan_t * plan,
    const double * y1s,
    double * y2s
) {
  const size_t n1 = plan->n1;
  const size_t n2 = plan->n2;
  pfunc_t * pfuncs = plan->pfuncs;
  point_t * points = plan->points;
  // find polynomial for each interval
  if (0 != fit_pfuncs(n1, y1s, pfuncs)) {
    return 1;
  }
  // interpolate
  if (0 != interpolate(n2, points, y2s)) {
    return 1;
  }
  return 0;
}

int interp_cubic_finalise (
    interp_cubic_plan_t ** plan
) {
  memory_free((*plan)->pfuncs);
  memory_free((*plan)->points);
  memory_free(*plan);
  *plan = NULL;
  return 0;
}

