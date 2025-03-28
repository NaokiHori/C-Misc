#if !defined(EQUATION_SOLVER_H)
#define EQUATION_SOLVER_H

#include <complex.h>

extern int solve_quadratic_equation(
    const double a0,
    const double a1,
    double complex roots[2]
);

extern int solve_cubic_equation(
    const double a0,
    const double a1,
    const double a2,
    double complex roots[3]
);

#endif // EQUATION_SOLVER_H
