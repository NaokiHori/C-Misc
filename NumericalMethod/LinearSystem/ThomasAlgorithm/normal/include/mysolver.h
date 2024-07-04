#if !defined(MYSOLVER_H)
#define MYSOLVER_H

typedef struct mysolver_internal_t mysolver_internal_t;

typedef struct mysolver_t {
  // size of linear system
  size_t size;
  // lower- / center- / upper-diagonal components
  // NOTE: l[0] and u[size - 1] will not be used
  double * restrict l;
  double * restrict c;
  double * restrict u;
  // input and output
  double * restrict q;
  // for internal use, opaque pointer
  mysolver_internal_t * internal;
} mysolver_t;

extern int mysolver_initialise (
    const size_t size,
    mysolver_t ** mysolver
);

extern int mysolver_solve (
    mysolver_t * mysolver
);

extern int mysolver_finalise (
    mysolver_t ** mysolver
);

#endif // MYSOLVER_H
