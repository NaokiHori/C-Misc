#include <complex.h>
#include <math.h>
#include "equation_solver.h"

int solve_quadratic_equation(
    const double a0,
    const double a1,
    double complex roots[2]
) {
  const double discriminant = a1 * a1 - 4. * a0;
  const double complex sqrt_d = discriminant < 0.
    ? sqrt(- discriminant) * I
    : sqrt(+ discriminant);
  roots[0] = a1 < 0.
    ? 2. * a0 / (-a1 + sqrt_d)
    : 0.5 * (- a1 - sqrt_d);
  roots[1] = 0. < a1
    ? 2. * a0 / (-a1 - sqrt_d)
    : 0.5 * (- a1 + sqrt_d);
  return 0;
}

int solve_cubic_equation(
    const double a0,
    const double a1,
    const double a2,
    double complex roots[3]
) {
  // https://en.wikipedia.org/wiki/Cubic_equation#Cardano's_method
  const double complex de_moivre[2] = {
    - 0.5 - 0.8660254037844386 * I,
    - 0.5 + 0.8660254037844386 * I,
  };
  // reformulate the given one into a depressed cubic equation:
  //   y^3 + py + q = 0
  // using the change of variable:
  //   y = x + a2 / 3
  const double p = a1 - 1. / 3. * pow(a2, 2.);
  const double q = 2. * pow(a2 / 3., 3.) - 1. / 3. * a1 * a2 + a0;
  const double p_third = p / 3.;
  const double q_half = q / 2.;
  const double p_third_cube = pow(p_third, 3.);
  const double q_half_square = pow(q_half, 2.);
  // x = - discriminant / 108
  const double x = p_third_cube + q_half_square;
  if (x < 0.) {
    // sqrt(x) yields a complex number
    const double complex sqrt_x = sqrt(-x) * I;
    const double complex u1 = - q_half + sqrt_x;
    const double complex u2 = - q_half - sqrt_x;
    const double complex cbrt_u1 = cpow(u1, 1. / 3.);
    const double complex cbrt_u2 = cpow(u2, 1. / 3.);
    roots[0] =                cbrt_u1 +                cbrt_u2;
    roots[1] = de_moivre[0] * cbrt_u1 + de_moivre[1] * cbrt_u2;
    roots[2] = de_moivre[1] * cbrt_u1 + de_moivre[0] * cbrt_u2;
  } else {
    // sqrt(x) yields a real number
    const double sqrt_x = sqrt(x);
    const double u1 = q <= 0.
      ? - q_half + sqrt_x
      : p_third_cube / (q_half + sqrt_x);
    const double u2 = 0. <= q
      ? - q_half - sqrt_x
      : p_third_cube / (q_half - sqrt_x);
    const double sign_u1 = u1 < 0. ? -1. : 1.;
    const double sign_u2 = u2 < 0. ? -1. : 1.;
    const double cbrt_u1 = sign_u1 * cbrt(sign_u1 * u1);
    const double cbrt_u2 = sign_u2 * cbrt(sign_u2 * u2);
    roots[0] =                cbrt_u1 +                cbrt_u2;
    roots[1] = de_moivre[0] * cbrt_u1 + de_moivre[1] * cbrt_u2;
    roots[2] = de_moivre[1] * cbrt_u1 + de_moivre[0] * cbrt_u2;
  }
  // transform y to x
  roots[0] -= a2 / 3.;
  roots[1] -= a2 / 3.;
  roots[2] -= a2 / 3.;
  return 0;
}

