#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include "dft.h"

static const double pi = 3.14159265358979324;

typedef struct {
  double cos;
  double sin;
} trig_t;

struct dft_plan_t {
  // degrees of freedom
  size_t nitems;
  // pre-computed trigonometric values
  trig_t * trigs;
  // pre-computed bit-reversed indices
  size_t * reversed_bit_indices;
};

static void * memory_alloc(
    const size_t size
) {
  void * const ptr = malloc(size);
  if (NULL == ptr) {
    fprintf(stderr, "[FATAL %s:%d] failed to allocate %zu bytes\n", __FILE__, __LINE__, size);
    exit(EXIT_FAILURE);
  }
  return ptr;
}

static void memory_free(
    void * const ptr
) {
  free(ptr);
}

static size_t get_reversed_bit_index(
    const size_t nitems,
    size_t index
) {
  size_t nbits = 0;
  {
    size_t n = nitems;
    while (n >>= 1) {
      nbits += 1;
    }
  }
  size_t reversed_index = 0;
  for (size_t bit = 0; bit < nbits; bit++) {
    reversed_index <<= 1;
    reversed_index |= index & 1;
    index >>= 1;
  }
  return reversed_index;
}

static int bit_reverse_inplace(
    const size_t nitems,
    const size_t * const reversed_bit_indices,
    double * const values
) {
  for (size_t i = 0; i < nitems; i++) {
    const size_t j = reversed_bit_indices[i];
    if (i < j) {
      const double tmp = values[i];
      values[i] = values[j];
      values[j] = tmp;
    }
  }
  return 0;
}

static int execute_stage(
    const size_t nitems,
    const trig_t * const trigs,
    const size_t stride,
    const double sign,
    double * const restrict reals,
    double * const restrict imags
) {
  const size_t block_size = 2 * stride;
  const size_t nblocks = nitems / block_size;
  for (size_t block_index = 0; block_index < nblocks; block_index++) {
    const size_t block_head = block_size * block_index;
    for (size_t k = 0; k < stride; k++) {
      const trig_t * const trig = trigs + nblocks * k;
      const double twiddle_real = trig->cos;
      const double twiddle_imag = trig->sin * sign;
      const size_t i = block_head + k;
      const size_t j = block_head + k + stride;
      const double real_i = reals[i];
      const double real_j = reals[j];
      const double imag_i = imags[i];
      const double imag_j = imags[j];
      const double real_j_twiddled = twiddle_real * real_j - twiddle_imag * imag_j;
      const double imag_j_twiddled = twiddle_imag * real_j + twiddle_real * imag_j;
      reals[i] = real_i + real_j_twiddled;
      reals[j] = real_i - real_j_twiddled;
      imags[i] = imag_i + imag_j_twiddled;
      imags[j] = imag_i - imag_j_twiddled;
    }
  }
  return 0;
}

static int scale(
    const size_t nitems,
    const double prefactor,
    double * const values
) {
  for (size_t i = 0; i < nitems; i++) {
    values[i] *= prefactor;
  }
  return 0;
}

static int execute_inplace_transform(
    const dft_plan_t * const plan,
    const double sign,
    double * const restrict reals,
    double * const restrict imags
) {
  const size_t nitems = plan->nitems;
  const trig_t * const trigs = plan->trigs;
  const size_t * const reversed_bit_indices = plan->reversed_bit_indices;
  bit_reverse_inplace(nitems, reversed_bit_indices, reals);
  bit_reverse_inplace(nitems, reversed_bit_indices, imags);
  for (size_t stride = 1; stride < nitems; stride <<= 1) {
    execute_stage(nitems, trigs, stride, sign, reals, imags);
  }
  return 0;
}

int dft_execute_inplace_forward_transform(
    const dft_plan_t * const plan,
    double * const restrict reals,
    double * const restrict imags
) {
  execute_inplace_transform(plan, -1., reals, imags);
  return 0;
}

int dft_execute_inplace_backward_transform(
    const dft_plan_t * const plan,
    double * const restrict reals,
    double * const restrict imags
) {
  execute_inplace_transform(plan, 1., reals, imags);
  const size_t nitems = plan->nitems;
  scale(nitems, 1. / nitems, reals);
  scale(nitems, 1. / nitems, imags);
  return 0;
}

int dft_init_plan(
    const size_t nitems,
    dft_plan_t ** const plan
) {
  if (nitems < 1) {
    fprintf(stderr, "nitems should be at least 1: %zu\n", nitems);
    return 1;
  }
  if (0 != (nitems & (nitems - 1))) {
    fprintf(stderr, "nitems should be a power of 2: %zu\n", nitems);
    return 1;
  }
  *plan = memory_alloc(1 * sizeof(dft_plan_t));
  (*plan)->nitems = nitems;
  trig_t ** const trigs = &(*plan)->trigs;
  size_t ** const reversed_bit_indices = &(*plan)->reversed_bit_indices;
  *trigs = memory_alloc(nitems / 2 * sizeof(trig_t));
  *reversed_bit_indices = memory_alloc(nitems * sizeof(size_t));
  // prepare cosine / sine tables
  for (size_t k = 0; k < nitems / 2; k++) {
    trig_t * const trig = *trigs + k;
    trig->cos = cos(2. * pi * k / nitems);
    trig->sin = sin(2. * pi * k / nitems);
  }
  // calculate bit-reversed order
  for (size_t index = 0; index < nitems; index++) {
    (*reversed_bit_indices)[index] = get_reversed_bit_index(nitems, index);
  }
  return 0;
}

int dft_destroy_plan(
    dft_plan_t ** const plan
) {
  memory_free((*plan)->trigs);
  memory_free((*plan)->reversed_bit_indices);
  memory_free(*plan);
  *plan = NULL;
  return 0;
}

