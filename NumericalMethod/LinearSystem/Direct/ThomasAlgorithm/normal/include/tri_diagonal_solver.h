#if !defined(TRI_DIAGONAL_SOLVER_H)
#define TRI_DIAGONAL_SOLVER_H

typedef struct tri_diagonal_solver_internal_t tri_diagonal_solver_internal_t;

typedef struct tri_diagonal_solver_t {
  // size of linear system
  size_t size;
  // lower- / center- / upper-diagonal components
  // NOTE: l[0] and u[size - 1] will not be used
  double * l;
  double * c;
  double * u;
  // input and output
  double * q;
  // for internal use, opaque pointer
  tri_diagonal_solver_internal_t * internal;
} tri_diagonal_solver_t;

extern int tri_diagonal_solver_initialise (
    const size_t size,
    tri_diagonal_solver_t ** const tri_diagonal_solver
);

extern int tri_diagonal_solver_solve (
    tri_diagonal_solver_t * const tri_diagonal_solver
);

extern int tri_diagonal_solver_finalise (
    tri_diagonal_solver_t ** const tri_diagonal_solver
);

#endif // TRI_DIAGONAL_SOLVER_H
