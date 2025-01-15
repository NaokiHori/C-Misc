// real-valued fast Fourier transform
// ref: https://www.kurims.kyoto-u.ac.jp/~ooura/fftman/ftmn2_12.html#sec2_1_2

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "rdft.h"

// NOTE: real and imaginary parts are stored separately
//   by virtue of the complex-conjugate nature
// e.g., N = 8, the last stage of the forward transform:
//   F0(0,2,4,6)
//   F1(0,2,4,6) <-+
//   F2(0,2,4,6)   |
//   F3(0,2,4,6) <-+-+
//   F0(1,3,5,7)   | |
//   F1(1,3,5,7) <-+ |
//   F2(1,3,5,7)     |
//   F3(1,3,5,7) <---+
// butterfly operations:
//   F1(0,2,4,6) and F1(1,3,5,7)
//   F3(0,2,4,6) and F3(1,3,5,7)
// complex conjugate to each other:
//   F1(0,2,4,6) and F3(0,2,4,6)
//   F1(1,3,5,7) and F3(1,3,5,7)

static const double pi = 3.1415926535897932384626;

typedef struct {
  double cos;
  double sin;
} trig_t;

struct rdft_plan_t {
  // degree of freedom
  size_t nitems;
  // pre-computed trigonometric values
  trig_t * trigs;
  // pre-computed bit-reversed indices
  size_t * reversed_bit_indices;
};

static void * memory_alloc (
    const size_t size
) {
  void * const ptr = malloc(size);
  if (NULL == ptr) {
    fprintf(stderr, "[FATAL %s:%d] failed to allocate %zu bytes\n", __FILE__, __LINE__, size);
    return NULL;
  }
  return ptr;
}

static void memory_free (
    void * const ptr
) {
  free(ptr);
}

static size_t get_nbits(
    size_t nitems
) {
  size_t nbits = 0;
  while (nitems >>= 1) {
    nbits += 1;
  }
  return nbits;
}

static size_t get_reversed_bit_index(
    const size_t nbits,
    size_t index
) {
  size_t reversed_index = 0;
  for (size_t bit = 0; bit < nbits; bit++) {
    reversed_index <<= 1;
    reversed_index |= index & 1;
    index >>= 1;
  }
  return reversed_index;
}

static int bit_reverse(
    const size_t nitems,
    const size_t * const reversed_bit_indices,
    double * const xs
) {
  for (size_t i = 0; i < nitems; i++) {
    const size_t j = reversed_bit_indices[i];
    if (i < j) {
      double * const xi = xs + i;
      double * const xj = xs + j;
      const double tmp = *xi;
      *xi = *xj;
      *xj = tmp;
    }
  }
  return 0;
}

int rdft_init_plan(
    const size_t nitems,
    rdft_plan_t ** const plan
) {
  *plan = NULL;
  if (nitems < 2) {
    printf("nitems (%zu) should be larger than 2\n", nitems);
    return 1;
  }
  if (nitems & (nitems - 1)) {
    printf("nitems (%zu) should be a power of 2\n", nitems);
    return 1;
  }
  *plan = memory_alloc(1 * sizeof(rdft_plan_t));
  (*plan)->nitems = nitems;
  trig_t ** trigs = &(*plan)->trigs;
  size_t ** reversed_bit_indices = &(*plan)->reversed_bit_indices;
  *trigs = memory_alloc(nitems / 2 * sizeof(trig_t));
  *reversed_bit_indices = memory_alloc(nitems * sizeof(size_t));
  // compute twiddle factors (wave numbers up to nitems / 2) a priori
  for (size_t k = 0; k < nitems / 2; k++) {
    trig_t * const trig = *trigs + k;
    trig->cos = cos(2. * pi * k / nitems);
    trig->sin = sin(2. * pi * k / nitems);
  }
  // calculate bit-reversed order a priori
  const size_t nbits = get_nbits(nitems);
  for (size_t index = 0; index < nitems; index++) {
    (*reversed_bit_indices)[index] = get_reversed_bit_index(nbits, index);
  }
  return 0;
}

int rdft_destroy_plan(
    rdft_plan_t ** const plan
) {
  memory_free((*plan)->trigs);
  memory_free((*plan)->reversed_bit_indices);
  memory_free(*plan);
  *plan = NULL;
  return 0;
}

int rdft_exec_f(
    rdft_plan_t * const plan,
    double * const xs
) {
  const size_t nitems = plan->nitems;
  const size_t * const reversed_bit_indices = plan->reversed_bit_indices;
  const trig_t * const trigs = plan->trigs;
  // pre-process
  const size_t nstages = get_nbits(nitems);
  bit_reverse(nitems, reversed_bit_indices, xs);
  // each stage of the butterfly operations
  for (size_t stage = 0; stage < nstages; stage++) {
    // stride: pointer difference of two elements
    //   between which butterfly operations are performed
    // stride = pow(2, stage)
    const size_t stride = 1 << stage;
    // there are several sub-systems called blocks,
    //   which are smaller systems in which DFTs aree performed
    const size_t block_size = stride << 1;
    const size_t n_block = nitems / block_size;
    for (size_t block_index = 0; block_index < n_block; block_index++) {
      const size_t block_head = block_size * block_index;
      // first wave number (k = 0): between two real numbers
      {
        const size_t i = block_head;
        const size_t j = block_head + stride;
        double * const xi = xs + i;
        double * const xj = xs + j;
        const double new_xi = *xi + *xj;
        const double new_xj = *xi - *xj;
        *xi = new_xi;
        *xj = new_xj;
      }
      // other wave numbers (k = 1, 2, ...): between two complex numbers
      // considered up to stride / 2 due to symmetry
      for (size_t k = 1; k < (stride >> 1); k++) {
        const size_t i_real = block_head + 0 * stride + k;
        const size_t i_imag = block_head + 1 * stride - k;
        const size_t j_real = block_head + 1 * stride + k;
        const size_t j_imag = block_head + 2 * stride - k;
        double * const xi_real = xs + i_real;
        double * const xi_imag = xs + i_imag;
        double * const xj_real = xs + j_real;
        double * const xj_imag = xs + j_imag;
        const size_t twiddle_index = nitems / block_size * k;
        const double twiddle_real = + trigs[twiddle_index].cos;
        const double twiddle_imag = - trigs[twiddle_index].sin;
        const double txj_real = + twiddle_real * *xj_real
                                + twiddle_imag * *xj_imag;
        const double txj_imag = - twiddle_real * *xj_imag
                                + twiddle_imag * *xj_real;
        const double new_xi_real = + *xi_real + txj_real;
        const double new_xi_imag = - *xi_imag + txj_imag;
        const double new_xj_real = + *xi_real - txj_real;
        const double new_xj_imag = - *xi_imag - txj_imag;
        *xi_real = + new_xi_real;
        *xi_imag = + new_xj_real;
        *xj_real = + new_xj_imag;
        *xj_imag = - new_xi_imag;
      }
    }
  }
  return 0;
}

int rdft_exec_b(
    rdft_plan_t * const plan,
    double * const xs
) {
  const size_t nitems = plan->nitems;
  const size_t * const reversed_bit_indices = plan->reversed_bit_indices;
  const trig_t * const trigs = plan->trigs;
  // each stage of the butterfly operations
  const size_t nstages = get_nbits(nitems);
  for (size_t stage = 0; stage < nstages; stage++) {
    // stride: pointer difference of two elements
    //   between which butterfly operations are performed
    // stride = pow(2, nstages - stage - 1)
    const size_t stride = 1 << (nstages - stage - 1);
    // there are several sub-systems called blocks,
    //   which are smaller systems in which DFTs aree performed
    const size_t block_size = stride << 1;
    const size_t n_block = nitems / block_size;
    for (size_t block_index = 0; block_index < n_block; block_index++) {
      const size_t block_head = block_size * block_index;
      // first wave number (k = 0): between two real numbers
      {
        const size_t i = block_head;
        const size_t j = block_head + stride;
        double * const xi = xs + i;
        double * const xj = xs + j;
        const double new_xi = *xi + *xj;
        const double new_xj = *xi - *xj;
        *xi = 0.5 * new_xi;
        *xj = 0.5 * new_xj;
      }
      // other wave numbers (k = 1, 2, ...): between two complex numbers
      // considered up to stride / 2 due to symmetry
      for (size_t k = 1; k < (stride >> 1); k++) {
        // real and imaginary parts are stored separately
        //   by virtue of the complex-conjugate nature
        const size_t i_real = block_head + 0 * stride + k;
        const size_t i_imag = block_head + 1 * stride - k;
        const size_t j_real = block_head + 1 * stride + k;
        const size_t j_imag = block_head + 2 * stride - k;
        double * const xi_real = xs + i_real;
        double * const xi_imag = xs + i_imag;
        double * const xj_real = xs + j_real;
        double * const xj_imag = xs + j_imag;
        const size_t twiddle_index = nitems / block_size * k;
        const double twiddle_real = + trigs[twiddle_index].cos;
        const double twiddle_imag = + trigs[twiddle_index].sin;
        const double new_xi_real = + *xi_real + *xi_imag;
        const double new_xi_imag = - *xj_real + *xj_imag;
        const double real = + *xi_real - *xi_imag;
        const double imag = + *xj_real + *xj_imag;
        const double new_xj_real = + twiddle_real * real
                                   + twiddle_imag * imag;
        const double new_xj_imag = + twiddle_real * imag
                                   - twiddle_imag * real;
        *xi_real = 0.5 * new_xi_real;
        *xi_imag = 0.5 * new_xi_imag;
        *xj_real = 0.5 * new_xj_real;
        *xj_imag = 0.5 * new_xj_imag;
      }
    }
  }
  // post-process
  bit_reverse(nitems, reversed_bit_indices, xs);
  return 0;
}

