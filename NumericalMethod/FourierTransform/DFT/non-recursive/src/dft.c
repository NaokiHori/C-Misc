#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include <complex.h>
#include "dft.h"

static const double pi = 3.14159265358979324;

typedef struct {
  double cos;
  double sin;
} trig_t;

struct dft_plan_t {
  // degree of freedom
  size_t nitems;
  // pre-computed trigonometric values
  trig_t * trigs;
  // internal buffer
  double complex * buf;
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

static size_t get_nbits (
    size_t n
) {
  size_t nbits = 0;
  while (n >>= 1) {
    nbits += 1;
  }
  return nbits;
}

static size_t get_reversed_bit_index (
    const size_t nbits,
    size_t n
) {
  size_t retval = 0;
  for (size_t i = 0; i < nbits; i++) {
    retval <<= 1;
    retval |= n & 1;
    n >>= 1;
  }
  return retval;
}

static int dft(
    const size_t nitems,
    const size_t stride,
    double complex * xs,
    double complex * ys
) {
  const size_t size_fft = 2 * stride;
  const size_t nbutterflies = nitems / size_fft;
  for (size_t butterfly = 0; butterfly < nbutterflies; butterfly += 1) {
    const size_t butterfly_start = size_fft * butterfly;
    for (size_t wave = 0; wave < stride; wave += 1) {
      const size_t i = butterfly_start + wave + 0;
      const size_t j = butterfly_start + wave + stride;
      const complex double twiddle = cexp(- 2. * pi * wave / size_fft * I);
      ys[i] = xs[i] + twiddle * xs[j];
      ys[j] = xs[i] - twiddle * xs[j];
    }
  }
  for (size_t i = 0; i < nitems; i++) {
    xs[i] = ys[i];
  }
  return 0;
}

static int idft(
    const size_t nitems,
    const size_t stride,
    double complex * xs,
    double complex * ys
) {
  const size_t size_fft = 2 * stride;
  const size_t nbutterflies = nitems / size_fft;
  for (size_t butterfly = 0; butterfly < nbutterflies; butterfly += 1) {
    const size_t butterfly_start = size_fft * butterfly;
    for (size_t wave = 0; wave < stride; wave += 1) {
      const size_t i = butterfly_start + wave + 0;
      const size_t j = butterfly_start + wave + stride;
      const complex double twiddle = cexp(+ 2. * pi * wave / size_fft * I);
      ys[i] = 0.5 * xs[i] + 0.5 * twiddle * xs[j];
      ys[j] = 0.5 * xs[i] - 0.5 * twiddle * xs[j];
    }
  }
  for (size_t i = 0; i < nitems; i++) {
    xs[i] = ys[i];
  }
  return 0;
}

int dft_exec_f (
    dft_plan_t * const plan,
    double complex * const xs
) {
  if (NULL == plan) {
    puts("uninitialized plan is passed");
    return 1;
  }
  const size_t nitems = plan->nitems;
  if (1 == nitems) {
    return 0;
  }
  double complex * const ys = plan->buf;
  // bit-reversal
  const size_t nbits = get_nbits(nitems);
  for (size_t i = 0; i < nitems; i++) {
    const size_t j = get_reversed_bit_index(nbits, i);
    ys[i] = xs[j];
  }
  for (size_t i = 0; i < nitems; i++) {
    xs[i] = ys[i];
  }
  for (size_t stride = 1; stride < nitems; stride <<= 1) {
    dft(nitems, stride, xs, ys);
  }
  return 0;
}

int dft_exec_b (
    dft_plan_t * const plan,
    double complex * const xs
) {
  if (NULL == plan) {
    puts("uninitialized plan is passed");
    return 1;
  }
  const size_t nitems = plan->nitems;
  double complex * const ys = plan->buf;
  // bit-reversal
  const size_t nbits = get_nbits(nitems);
  for (size_t i = 0; i < nitems; i++) {
    const size_t j = get_reversed_bit_index(nbits, i);
    ys[i] = xs[j];
  }
  for (size_t i = 0; i < nitems; i++) {
    xs[i] = ys[i];
  }
  for (size_t stride = 1; stride < nitems; stride <<= 1) {
    idft(nitems, stride, xs, ys);
  }
  return 0;
}

int dft_init_plan (
    const size_t nitems,
    dft_plan_t ** const plan
) {
  if (0 != (nitems & (nitems - 1))) {
    fprintf(stderr, "nitems should be a power of 2: %zu\n", nitems);
    return 1;
  }
  *plan = memory_alloc(1 * sizeof(dft_plan_t));
  (*plan)->nitems = nitems;
  trig_t ** const trigs = &(*plan)->trigs;
  double complex ** const buf = &(*plan)->buf;
  *trigs = memory_alloc(nitems * sizeof(trig_t));
  *buf = memory_alloc(nitems * sizeof(double complex));
  // prepare cosine / sine tables
  for (size_t i = 0; i < nitems; i++) {
    trig_t * const trig = *trigs + i;
    trig->cos = cos(2. * pi * i / nitems);
    trig->sin = sin(2. * pi * i / nitems);
  }
  return 0;
}

// clean-up a plan
int dft_destroy_plan (
    dft_plan_t ** const plan
) {
  memory_free((*plan)->trigs);
  memory_free((*plan)->buf);
  memory_free(*plan);
  *plan = NULL;
  return 0;
}

