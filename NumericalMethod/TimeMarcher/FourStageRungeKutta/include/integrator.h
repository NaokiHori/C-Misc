#if !defined(INTEGRATOR_H)
#define INTEGRATOR_H

#include <stddef.h>
#include "type.h"

#define RK_ORDER 4

typedef struct {
  size_t ndims;
  real_t * buf_x;
  real_t * buf_dx[RK_ORDER];
  int (* kernel)(
    const real_t * const x,
    real_t * const dx
  );
} integrator_t;

extern int integrator_initialize(
    const size_t ndims,
    int (* const kernel)(
      const real_t * const x,
      real_t * const dx
    ),
    integrator_t * const integrator
);

extern int integrator_update(
    integrator_t * const integrator,
    const real_t dt,
    real_t * const position
);

extern int integrator_finalize(
    integrator_t * const integrator
);

#endif // INTEGRATOR_H
