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

static int bit_reverse_in_place(
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

static int bit_reverse_out_of_place(
    const size_t nitems,
    const size_t * const reversed_bit_indices,
    const double * const xs,
    double * const ys
) {
  for (size_t i = 0; i < nitems; i++) {
    const size_t j = reversed_bit_indices[i];
    ys[i] = xs[j];
  }
  return 0;
}

int rdft_init_plan(
    const size_t nitems,
    rdft_plan_t ** const plan
) {
  *plan = NULL;
  if (nitems < 2) {
    printf("nitems (%zu) should be at least 2\n", nitems);
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
  *trigs = memory_alloc(nitems / 4 * sizeof(trig_t));
  *reversed_bit_indices = memory_alloc(nitems * sizeof(size_t));
  // compute twiddle factors (wavenumbers up to nitems / 4) a priori
  for (size_t k = 0; k < nitems / 4; k++) {
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
    const rdft_plan_t * const plan,
    const double * const xs,
    double * const ys
) {
  const size_t nitems = plan->nitems;
  const size_t * const reversed_bit_indices = plan->reversed_bit_indices;
  const trig_t * const trigs = plan->trigs;
  // pre-process
  const size_t nstages = get_nbits(nitems);
  if (xs == ys) {
    bit_reverse_in_place(nitems, reversed_bit_indices, ys);
  } else {
    bit_reverse_out_of_place(nitems, reversed_bit_indices, xs, ys);
  }
  // each stage of the butterfly operations
  for (size_t stage = 0; stage < nstages; stage++) {
    // stride: pointer difference of two elements
    //   between which butterfly operations are performed
    const size_t stride = (size_t)1 << stage;
    // there are several sub-systems called blocks,
    //   which are smaller systems in which DFTs are performed
    const size_t block_size = stride << 1;
    const size_t n_block = nitems / block_size;
    for (size_t block_index = 0; block_index < n_block; block_index++) {
      const size_t block_head = block_size * block_index;
      // first wavenumber (k = 0): between two real numbers
      {
        const size_t i = block_head;
        const size_t j = block_head + stride;
        double * const yi = ys + i;
        double * const yj = ys + j;
        const double new_yi = *yi + *yj;
        const double new_yj = *yi - *yj;
        *yi = new_yi;
        *yj = new_yj;
      }
      // other wavenumbers (k = 1, 2, ...): between two complex numbers
      // considered up to stride / 2 due to symmetry
      for (size_t k = 1; k < (stride >> 1); k++) {
        const size_t i_real = block_head + 0 * stride + k;
        const size_t i_imag = block_head + 1 * stride - k;
        const size_t j_real = block_head + 1 * stride + k;
        const size_t j_imag = block_head + 2 * stride - k;
        double * const yi_real = ys + i_real;
        double * const yi_imag = ys + i_imag;
        double * const yj_real = ys + j_real;
        double * const yj_imag = ys + j_imag;
        const size_t twiddle_index = nitems / block_size * k;
        const double twiddle_real = + trigs[twiddle_index].cos;
        const double twiddle_imag = - trigs[twiddle_index].sin;
        const double tyj_real = + twiddle_real * *yj_real
                                + twiddle_imag * *yj_imag;
        const double tyj_imag = - twiddle_real * *yj_imag
                                + twiddle_imag * *yj_real;
        const double new_yi_real = + *yi_real + tyj_real;
        const double new_yi_imag = - *yi_imag + tyj_imag;
        const double new_yj_real = + *yi_real - tyj_real;
        const double new_yj_imag = - *yi_imag - tyj_imag;
        *yi_real = + new_yi_real;
        *yi_imag = + new_yj_real;
        *yj_real = + new_yj_imag;
        *yj_imag = - new_yi_imag;
      }
    }
  }
  return 0;
}

int rdft_exec_b(
    const rdft_plan_t * const plan,
    const double * const xs,
    double * const ys
) {
  const size_t nitems = plan->nitems;
  const size_t * const reversed_bit_indices = plan->reversed_bit_indices;
  const trig_t * const trigs = plan->trigs;
  // each stage of the butterfly operations
  const size_t nstages = get_nbits(nitems);
  for (size_t stage = 0; stage < nstages; stage++) {
    const double * const inputs = 0 == stage ? xs : ys;
    double * const outputs = ys;
    // stride: pointer difference of two elements
    //   between which butterfly operations are performed
    const size_t stride = (size_t)1 << (nstages - stage - 1);
    // there are several sub-systems called blocks,
    //   which are smaller systems in which DFTs are performed
    const size_t block_size = stride << 1;
    const size_t n_block = nitems / block_size;
    for (size_t block_index = 0; block_index < n_block; block_index++) {
      const size_t block_head = block_size * block_index;
      // first wavenumber (k = 0): between two real numbers
      {
        const size_t i = block_head;
        const size_t j = block_head + stride;
        const double input_i = inputs[i];
        const double input_j = inputs[j];
        outputs[i] = 0.5 * (input_i + input_j);
        outputs[j] = 0.5 * (input_i - input_j);
      }
      // other wavenumbers (k = 1, 2, ...): between two complex numbers
      // considered up to stride / 2 due to symmetry
      for (size_t k = 1; k < (stride >> 1); k++) {
        // real and imaginary parts are stored separately
        //   by virtue of the complex-conjugate nature
        const size_t i_real = block_head + 0 * stride + k;
        const size_t i_imag = block_head + 1 * stride - k;
        const size_t j_real = block_head + 1 * stride + k;
        const size_t j_imag = block_head + 2 * stride - k;
        const double input_i_real = inputs[i_real];
        const double input_i_imag = inputs[i_imag];
        const double input_j_real = inputs[j_real];
        const double input_j_imag = inputs[j_imag];
        const size_t twiddle_index = nitems / block_size * k;
        const double twiddle_real = + trigs[twiddle_index].cos;
        const double twiddle_imag = + trigs[twiddle_index].sin;
        const double real = + input_i_real - input_i_imag;
        const double imag = + input_j_real + input_j_imag;
        outputs[i_real] = 0.5 * (+ input_i_real        + input_i_imag       );
        outputs[i_imag] = 0.5 * (- input_j_real        + input_j_imag       );
        outputs[j_real] = 0.5 * (+ twiddle_real * real + twiddle_imag * imag);
        outputs[j_imag] = 0.5 * (+ twiddle_real * imag - twiddle_imag * real);
      }
      // nyquist wavenumber: just copy
      if (0 == stage) {
        const size_t i = block_head + (stride >> 1);
        const size_t j = i + stride;
        outputs[i] = inputs[i];
        outputs[j] = inputs[j];
      }
    }
  }
  // post-process
  bit_reverse_in_place(nitems, reversed_bit_indices, ys);
  return 0;
}

