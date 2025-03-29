#include <assert.h>
#include <complex.h>
#include <float.h>
#include <math.h>
#include <stddef.h>
#include <stdio.h>
#include "equation_solver.h"

static int set_quadratic_coefficients(
    const double complex roots[2],
    double coefs[2]
) {
  const double complex c0 = roots[0] * roots[1];
  const double complex c1 = - roots[0] - roots[1];
  assert(fabs(cimag(c0)) < DBL_EPSILON);
  assert(fabs(cimag(c1)) < DBL_EPSILON);
  coefs[0] = creal(c0);
  coefs[1] = creal(c1);
  return 0;
}

static int set_cubic_coefficients(
    const double complex roots[3],
    double coefs[3]
) {
  const double complex c0 = - roots[0] * roots[1] * roots[2];
  const double complex c1 = + roots[0] * roots[1]
                            + roots[1] * roots[2]
                            + roots[2] * roots[0];
  const double complex c2 = - roots[0] - roots[1] - roots[2];
  assert(fabs(cimag(c0)) < DBL_EPSILON);
  assert(fabs(cimag(c1)) < DBL_EPSILON);
  assert(fabs(cimag(c2)) < DBL_EPSILON);
  coefs[0] = creal(c0);
  coefs[1] = creal(c1);
  coefs[2] = creal(c2);
  return 0;
}

static int check(
    const size_t ndims,
    const double complex * const answers,
    const double complex * const results
) {
  printf("Solution of %s equation (root and error)\n", 2 == ndims ? "quadratic" : 3 == ndims ? "cubic" : "N/A");
  for (size_t n = 0; n < ndims; n++) {
    const double complex result = results[n];
    const double complex answer = answers[n];
    const double complex error = result - answer;
    printf(
        "  %zu (% .15e + % .15e * I) (% .1e + % .1e * I)\n",
        n, creal(result), cimag(result), creal(error), cimag(error)
    );
  }
  return 0;
}

static int use_quadratic_solver(
    void
) {
  double complex roots[2] = {
    0. + 0. * I,
    0. + 0. * I,
  };
  double coefs[2] = {
    0.,
    0.,
  };
  {
    // Two distinct real roots
    const double complex answers[2] = {
      1.,
      2.,
    };
    set_quadratic_coefficients(answers, coefs);
    solve_quadratic_equation(coefs[0], coefs[1], roots);
    check(2, answers, roots);
  }
  {
    // Two repeated real roots
    const double complex answers[2] = {
      1.,
      1.,
    };
    set_quadratic_coefficients(answers, coefs);
    solve_quadratic_equation(coefs[0], coefs[1], roots);
    check(2, answers, roots);
  }
  {
    // Two real roots very close to each other
    const double complex answers[2] = {
      1. + DBL_EPSILON,
      1.,
    };
    set_quadratic_coefficients(answers, coefs);
    solve_quadratic_equation(coefs[0], coefs[1], roots);
    check(2, answers, roots);
  }
  {
    // Two complex roots
    const double complex answers[2] = {
      0.5 * (-1. - sqrt(3.) * I),
      0.5 * (-1. + sqrt(3.) * I),
    };
    set_quadratic_coefficients(answers, coefs);
    solve_quadratic_equation(coefs[0], coefs[1], roots);
    check(2, answers, roots);
  }
  {
    // Two imaginary roots
    const double complex answers[2] = {
      - 3.14 * I,
      + 3.14 * I,
    };
    set_quadratic_coefficients(answers, coefs);
    solve_quadratic_equation(coefs[0], coefs[1], roots);
    check(2, answers, roots);
  }
  {
    // Two real roots which are totally different in ratio
    const double complex answers[2] = {
         0.001223991124941416117107294,
      1633.998776008875058583882892705,
    };
    set_quadratic_coefficients(answers, coefs);
    solve_quadratic_equation(coefs[0], coefs[1], roots);
    check(2, answers, roots);
  }
  return 0;
}

static int use_cubic_solver(
    void
) {
  // examples are taken from: https://www.mathcentre.ac.uk/resources/uploaded/mc-ty-cubicequations-2009-1.pdf
  double complex roots[3] = {
    0. + 0. * I,
    0. + 0. * I,
    0. + 0. * I,
  };
  double coefs[3] = {
    0.,
    0.,
    0.,
  };
  {
    // Three distinct real roots
    const double complex answers[3] = {
      3.,
      2.,
      1.,
    };
    set_cubic_coefficients(answers, coefs);
    solve_cubic_equation(coefs[0], coefs[1], coefs[2], roots);
    check(3, answers, roots);
  }
  {
    // One distinct and two repeated real roots
    const double complex answers[3] = {
      2.,
      2.,
      1.,
    };
    set_cubic_coefficients(answers, coefs);
    solve_cubic_equation(coefs[0], coefs[1], coefs[2], roots);
    check(3, answers, roots);
  }
  {
    // Three repeated real roots
    const double complex answers[3] = {
      1.,
      1.,
      1.,
    };
    set_cubic_coefficients(answers, coefs);
    solve_cubic_equation(coefs[0], coefs[1], coefs[2], roots);
    check(3, answers, roots);
  }
  {
    // One real and two complex roots
    const double complex answers[3] = {
      1.,
      - 1. - sqrt(2.) * I,
      - 1. + sqrt(2.) * I,
    };
    set_cubic_coefficients(answers, coefs);
    solve_cubic_equation(coefs[0], coefs[1], coefs[2], roots);
    check(3, answers, roots);
  }
  return 0;
}

int main(
    void
) {
  use_quadratic_solver();
  use_cubic_solver();
  return 0;
}

