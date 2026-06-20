#include <stdio.h>
#include <math.h>
#include <assert.h>
#include "tri_diagonal_solver.h"

// solve the following
// -2  1  0  0 | -3
//  1 -2  1  0 | -1
//  0  1 -2  1 | +1
//  0  0  1 -2 | +3
// whose answer is (+2, +1, -1, -2)
int main (
    void
) {
#define SIZE 4
  tri_diagonal_solver_t * tri_diagonal_solver = NULL;
  assert(0 == tri_diagonal_solver_initialise(SIZE, &tri_diagonal_solver));
  // set tri-diagonal matrix
  double * l = tri_diagonal_solver->l;
  double * c = tri_diagonal_solver->c;
  double * u = tri_diagonal_solver->u;
  for (size_t n = 0; n < SIZE; n++) {
    l[n] = + 1.;
    c[n] = - 2.;
    u[n] = + 1.;
  }
  // set input
  double * q = tri_diagonal_solver->q;
  q[0] = - 3.;
  q[1] = - 1.;
  q[2] = + 1.;
  q[3] = + 3.;
  assert(0 == tri_diagonal_solver_solve(tri_diagonal_solver));
  // compare
  const double answer[SIZE] = {2., 1., -1., -2.};
  double error = 0;
  for (size_t n = 0; n < SIZE; n++) {
    error += pow(q[n] - answer[n], 2.);
  }
  printf("error: % .1e\n", error);
  assert(0 == tri_diagonal_solver_finalise(&tri_diagonal_solver));
  return 0;
}

