#if !defined(VECTOR_H)
#define VECTOR_H

#define NDIMS 2

typedef double vector_t[NDIMS + 1];

typedef struct {
  double (* const inner_product)(
      const vector_t * const a,
      const vector_t * const b
  );
} vector_op_t;

extern const vector_op_t vector_op;

#endif // VECTOR_H
