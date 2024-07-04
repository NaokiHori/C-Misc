#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <assert.h>
#include "mysolver.h"

// solve the following
// -4  1  0  1 | -6
//  1 -4  1  0 | +6
//  0  1 -4  1 | -6
//  1  0  1 -4 | +6
// whose answer is (+1, -1, 1, -1)
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
    c[n] = - 4.;
    u[n] = + 1.;
  }
  // set input
  double * q = mysolver->q;
  q[0] = - 6.;
  q[1] = + 6.;
  q[2] = - 6.;
  q[3] = + 6.;
  assert(0 == mysolver_solve(mysolver));
  // compare
  const double answer[SIZE] = {1., -1., 1., -1.};
  double error = 0;
  for (size_t n = 0; n < SIZE; n++) {
    error += pow(q[n] - answer[n], 2.);
  }
  printf("error: % .1e\n", error);
  assert(0 == mysolver_finalise(&mysolver));
  return 0;
}

