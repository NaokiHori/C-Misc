#include <stdio.h>
#include <math.h>
#include <assert.h>
#include "mysolver.h"

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
  mysolver_t * mysolver = NULL;
  assert(0 == mysolver_initialise(SIZE, &mysolver));
  // set tri-diagonal matrix
  double * l = mysolver->l;
  double * c = mysolver->c;
  double * u = mysolver->u;
  for (size_t n = 0; n < SIZE; n++) {
    l[n] = + 1.;
    c[n] = - 2.;
    u[n] = + 1.;
  }
  // set input
  double * q = mysolver->q;
  q[0] = - 3.;
  q[1] = - 1.;
  q[2] = + 1.;
  q[3] = + 3.;
  assert(0 == mysolver_solve(mysolver));
  // compare
  const double answer[SIZE] = {2., 1., -1., -2.};
  double error = 0;
  for (size_t n = 0; n < SIZE; n++) {
    error += pow(q[n] - answer[n], 2.);
  }
  printf("error: % .1e\n", error);
  assert(0 == mysolver_finalise(&mysolver));
  return 0;
}

