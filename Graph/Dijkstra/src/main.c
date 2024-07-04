#include <stdlib.h>
#include "init.h"
#include "dijkstra.h"

int main (
    void
) {
  const size_t nnodes = 768;
  size_t nlinks = 0;
  link_t * links = NULL;
  if (0 != init_graph(nnodes, &nlinks, &links)) {
    exit(EXIT_FAILURE);
  }
  if (0 != dijkstra(nnodes, nlinks, links)) {
    exit(EXIT_FAILURE);
  }
  return 0;
}

