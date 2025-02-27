#include <stdio.h>
#include "inverse3x3.h"

int main(
    void
) {
  double matrix[9] = {
    +1., +1., -1.,
    -2., -1., +1.,
    -1., -2., +1.,
  };
  if (0 != find_inverse_matrix(matrix)) {
    puts("failed to find inverse matrix");
    return 1;
  }
  printf("% .7e % .7e % .7e\n", matrix[0], matrix[1], matrix[2]);
  printf("% .7e % .7e % .7e\n", matrix[3], matrix[4], matrix[5]);
  printf("% .7e % .7e % .7e\n", matrix[6], matrix[7], matrix[8]);
  return 0;
}

