#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "rdft.h"

#define REALIMAG 2

static const double pi = 3.14159265358979324;

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
  size_t * rev_indices;
  // internal buffer
  double * buf;
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

static size_t find_pair_index(
    const size_t stride,
    const size_t i
) {
  return i ^ stride;
}

static int dft(
    const size_t nitems,
    const trig_t * const trigs,
    const size_t * const rev_indices,
    const size_t stride,
    double * xs,
    double * ys
) {
  const size_t size_fft = 2 * stride;
  for (size_t i = 0; i < REALIMAG * nitems; i += 1) {
    const int is_real = 0 == i % REALIMAG;
    const size_t k0 = i / 2;
    const size_t k1 = k0 ^ stride;
    const double x0_r = xs[2 * k0 + 0];
    const double x0_i = xs[2 * k0 + 1];
    const double x1_r = xs[2 * k1 + 0];
    const double x1_i = xs[2 * k1 + 1];
    const size_t butterfly_index = k0 / size_fft;
    const size_t k = rev_indices[butterfly_index] >> 1;
    const double twiddle_r = + trigs[2 * k].cos;
    const double twiddle_i = - trigs[2 * k].sin;
    if (k0 < k1) {
      if (is_real) {
        ys[i] = x0_r + x1_r;
      } else {
        ys[i] = x0_i + x1_i;
      }
    } else {
      const double v_r = x1_r - x0_r;
      const double v_i = x1_i - x0_i;
      if (is_real) {
        ys[i] = twiddle_r * v_r - twiddle_i * v_i;
      } else {
        ys[i] = twiddle_r * v_i + twiddle_i * v_r;
      }
    }
  }
  for (size_t i = 0; i < 2 * nitems; i++) {
    xs[i] = ys[i];
  }
  return 0;
}

static int idft(
    const size_t nitems,
    const trig_t * const trigs,
    const size_t * const rev_indices,
    const size_t stride,
    double * xs,
    double * ys
) {
  const size_t size_fft = 2 * stride;
  for (size_t i = 0; i < 2 * nitems; i += 1) {
    const int is_real = 0 == i % 2;
    const size_t k0 = i / 2;
    const size_t k1 = find_pair_index(stride, k0);
    const double x0_r = 0.5 * xs[2 * k0 + 0];
    const double x0_i = 0.5 * xs[2 * k0 + 1];
    const double x1_r = 0.5 * xs[2 * k1 + 0];
    const double x1_i = 0.5 * xs[2 * k1 + 1];
    const size_t butterfly_index = k0 / size_fft;
    const size_t k = rev_indices[butterfly_index] >> 1;
    const double twiddle_r = + trigs[2 * k].cos;
    const double twiddle_i = + trigs[2 * k].sin;
    if (k0 < k1) {
      if (is_real) {
        ys[i] = x0_r + x1_r;
      } else {
        ys[i] = x0_i + x1_i;
      }
    } else {
      const double v_r = x1_r - x0_r;
      const double v_i = x1_i - x0_i;
      if (is_real) {
        ys[i] = twiddle_r * v_r - twiddle_i * v_i;
      } else {
        ys[i] = twiddle_r * v_i + twiddle_i * v_r;
      }
    }
  }
  for (size_t i = 0; i < 2 * nitems; i++) {
    xs[i] = ys[i];
  }
  return 0;
}

int rdft_exec_f (
    rdft_plan_t * const plan,
    double * const xs
) {
  if (NULL == plan) {
    puts("uninitialized plan is passed");
    return 1;
  }
  const size_t nitems = plan->nitems;
  const trig_t * const trigs = plan->trigs;
  const size_t * const rev_indices = plan->rev_indices;
  const size_t nh = nitems / 2;
  double * const zs = plan->buf;
  // reorder input signal to be the bit-reversal order
  for (size_t i = 0; i < nitems; i++) {
    const int is_real = 0 == i % 2;
    const size_t j = rev_indices[i / 2];
    zs[i] = xs[2 * j + (is_real ? 0 : 1)];
  }
  for (size_t i = 0; i < nitems; i++) {
    xs[i] = zs[i];
  }
  // perform complex DFT with halved size
  for (size_t stride = 1; stride < nh; stride <<= 1) {
    dft(nh, trigs, rev_indices, stride, xs, zs);
  }
  for (size_t i = 0; i < nitems; i++) {
    zs[i] = xs[i];
  }
  // reorder to get output signal
  xs[0] = zs[0] + zs[1];
  for (size_t k = 1; k < nh; k++) {
    const double twiddle_r = + trigs[k].cos;
    const double twiddle_i = - trigs[k].sin;
    const double z0_r = + 0.5 * zs[2 * k + 0];
    const double z0_i = + 0.5 * zs[2 * k + 1];
    const double z1_r = + 0.5 * zs[2 * (nh - k) + 0];
    const double z1_i = + 0.5 * zs[2 * (nh - k) + 1];
    const double xe_r = + z0_r + z1_r;
    const double xo_r = + z0_i + z1_i;
    const double xo_i = - z0_r + z1_r;
    const double xot_r = xo_r * twiddle_r - xo_i * twiddle_i;
    xs[k] = xe_r + xot_r;
  }
  xs[nh] = zs[0] - zs[1];
  for (size_t k = nh + 1; k < nitems; k++) {
    const double twiddle_r = + trigs[k - nh].cos;
    const double twiddle_i = - trigs[k - nh].sin;
    const double z0_r = + 0.5 * zs[2 * (k - nh) + 0];
    const double z0_i = + 0.5 * zs[2 * (k - nh) + 1];
    const double z1_r = + 0.5 * zs[2 * (nitems - k) + 0];
    const double z1_i = + 0.5 * zs[2 * (nitems - k) + 1];
    const double xe_i = + z0_i - z1_i;
    const double xo_r = + z0_i + z1_i;
    const double xo_i = - z0_r + z1_r;
    const double xot_i = xo_r * twiddle_i + xo_i * twiddle_r;
    xs[k] = xe_i - xot_i;
  }
  return 0;
}

int rdft_exec_b (
    rdft_plan_t * const plan,
    double * const xs
) {
  if (NULL == plan) {
    puts("uninitialized plan is passed");
    return 1;
  }
  const size_t nitems = plan->nitems;
  const trig_t * const trigs = plan->trigs;
  const size_t * const rev_indices = plan->rev_indices;
  const size_t nh = nitems / 2;
  double * const zs = plan->buf;
  // create a complex signal
  zs[0] = 0.5 * xs[0] + 0.5 * xs[nh];
  zs[1] = 0.5 * xs[0] - 0.5 * xs[nh];
  for (size_t k = 2; k < nitems; k++) {
    const size_t l = k / 2;
    const double twiddle_r = trigs[l].cos;
    const double twiddle_i = trigs[l].sin;
    const double x0_r = + 0.5 * xs[       + l];
    const double x0_i = - 0.5 * xs[nitems - l];
    const double x1_r = + 0.5 * xs[    nh - l];
    const double x1_i = + 0.5 * xs[    nh + l];
    if (0 == k % 2) {
      const double xe_r = x0_r + x1_r;
      const double xo_i = (x0_r - x1_r) * twiddle_i + (x0_i - x1_i) * twiddle_r;
      zs[k] = xe_r - xo_i;
    } else {
      const double xe_i = x0_i + x1_i;
      const double xo_r = (x0_r - x1_r) * twiddle_r - (x0_i - x1_i) * twiddle_i;
      zs[k] = xe_i + xo_r;
    }
  }
  for (size_t i = 0; i < nitems; i++) {
    xs[i] = zs[i];
  }
  // reorder signal to be bit-reversed order
  for (size_t i = 0; i < nitems; i++) {
    const int is_real = 0 == i % 2;
    const size_t j = rev_indices[i / 2];
    zs[i] = xs[2 * j + (is_real ? 0 : 1)];
  }
  for (size_t i = 0; i < nitems; i++) {
    xs[i] = zs[i];
  }
  // perform inverse complex DFT with halved size
  for (size_t stride = 1; stride < nh; stride <<= 1) {
    idft(nh, trigs, rev_indices, stride, xs, zs);
  }
  for (size_t i = 0; i < nitems; i++) {
    xs[i] = zs[i];
  }
  return 0;
}

int rdft_init_plan (
    const size_t nitems,
    rdft_plan_t ** const plan
) {
  if (0 != nitems % 2) {
    printf("signal length (%zu) should be a multiple of 2\n", nitems);
    return 1;
  }
  *plan = memory_alloc(1 * sizeof(rdft_plan_t));
  (*plan)->nitems = nitems;
  if (nitems & (nitems - 1)) {
    printf("nitems (%zu) should be a power of 2\n", nitems);
    return 1;
  }
  trig_t ** const trigs = &(*plan)->trigs;
  size_t ** rev_indices = &(*plan)->rev_indices;
  double ** const buf = &(*plan)->buf;
  const size_t nh = nitems / 2;
  *trigs = memory_alloc(nh * sizeof(trig_t));
  *rev_indices = memory_alloc(nh * sizeof(size_t));
  *buf = memory_alloc(nitems * sizeof(double));
  // prepare trigonometric tables
  for (size_t i = 0; i < nh; i++) {
    trig_t * const trig = *trigs + i;
    trig->cos = cos(2. * pi * i / nitems);
    trig->sin = sin(2. * pi * i / nitems);
  }
  // prepare bit-reversal indices
  (*rev_indices)[0] = 0;
  for (size_t i = 1, j = 0; i < nh; i++) {
    for (size_t k = nh >> 1; k > (j ^= k); k >>= 1);
    (*rev_indices)[i] = j;
  }
  return 0;
}

// clean-up a plan
int rdft_destroy_plan (
    rdft_plan_t ** const plan
) {
  memory_free((*plan)->trigs);
  memory_free((*plan)->rev_indices);
  memory_free((*plan)->buf);
  memory_free(*plan);
  *plan = NULL;
  return 0;
}

