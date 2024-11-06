#include <stdio.h>
#include <stdlib.h>
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

/**
 * @brief Recursive radix-2 Cooley-Tukey FFT for complex input/output, forward transform
 * @param nitems Degree of freedom of the given signal ("N" in the definition of complex DFT)
 * @param stride Distance between two successive input elements: 2^(level of recursion)
 * @param trigs  Pre-computed twiddle factors (trigonometric functions)
 * @param xs     Input  signal ordered as Re(0), Im(0), Re(1), Im(1), ...
 * @param ys     Output signal ordered as Re(0), Im(0), Re(1), Im(1), ...
 * @return success (0) or failure (1)
 */
// main function to perform discrete fourier transform | 64
static int dft (
    const size_t nitems,
    const size_t stride,
    const trig_t * const trigs,
    const double complex * const xs,
    double complex * const ys
) {
  if (1 == nitems) {
    ys[0] = xs[0];
  } else if (0 == nitems % 2) {
    // size of sub problem
    const size_t nitems_sub = nitems / 2;
    // divide-and-conquer
    dft(nitems_sub, stride * 2, trigs, xs + 0 * stride, ys + 0 * nitems_sub);
    dft(nitems_sub, stride * 2, trigs, xs + 1 * stride, ys + 1 * nitems_sub);
    // unify sub problem results
    for (size_t k = 0; k < nitems_sub; k++) {
      const trig_t * const trig_y1 = trigs + stride * 1 * k;
      const double complex c_y1 = trig_y1->cos - I * trig_y1->sin;
      double complex * const y0_ptr = ys + k + 0 * nitems_sub;
      double complex * const y1_ptr = ys + k + 1 * nitems_sub;
      const double complex y0 = *y0_ptr;
      const double complex y1 = *y1_ptr * c_y1;
      *y0_ptr = 1. * y0 + 1. * y1;
      *y1_ptr = 1. * y0 - 1. * y1;
    }
  } else if (0 == nitems % 3) {
    // size of sub problem
    const size_t nitems_sub = nitems / 3;
    // divide-and-conquer
    dft(nitems_sub, stride * 3, trigs, xs + 0 * stride, ys + 0 * nitems_sub);
    dft(nitems_sub, stride * 3, trigs, xs + 1 * stride, ys + 1 * nitems_sub);
    dft(nitems_sub, stride * 3, trigs, xs + 2 * stride, ys + 2 * nitems_sub);
    // unify sub problem results
    for (size_t k = 0; k < nitems_sub; k++) {
      const trig_t * const trig_y1 = trigs + stride * 1 * k;
      const trig_t * const trig_y2 = trigs + stride * 2 * k;
      const double complex c_y1 = trig_y1->cos - I * trig_y1->sin;
      const double complex c_y2 = trig_y2->cos - I * trig_y2->sin;
      double complex * const y0_ptr = ys + k + 0 * nitems_sub;
      double complex * const y1_ptr = ys + k + 1 * nitems_sub;
      double complex * const y2_ptr = ys + k + 2 * nitems_sub;
      const double complex y0 = *y0_ptr;
      const double complex y1 = *y1_ptr * c_y1;
      const double complex y2 = *y2_ptr * c_y2;
      const double complex m = - 0.5 - 0.8660254037844386 * I;
      const double complex p = - 0.5 + 0.8660254037844386 * I;
      *y0_ptr = y0 +     y1 +     y2;
      *y1_ptr = y0 + m * y1 + p * y2;
      *y2_ptr = y0 + p * y1 + m * y2;
    }
  } else {
    // naive O(N^2) DFT
    for (size_t k = 0; k < nitems; k++) {
      double complex * const y = ys + k;
      *y = 0. + I * 0.;
      for (size_t n = 0; n < nitems; n++) {
        const size_t index = n * k % nitems;
        *y += xs[stride * n] * cexp(- 2. * pi * index * I / nitems);
      }
    }
  }
  return 0;
}

/**
 * @brief Recursive radix-2 Cooley-Tukey FFT for complex input/output, backward transform
 * @param nitems Degree of freedom of the given signal ("N" in the definition of complex DFT)
 * @param stride Distance between two successive input elements: 2^(level of recursion)
 * @param trigs  Pre-computed twiddle factors (trigonometric functions)
 * @param xs     Input  signal ordered as Re(0), Im(0), Re(1), Im(1), ...
 * @param ys     Output signal ordered as Re(0), Im(0), Re(1), Im(1), ...
 * @return success (0) or failure (1)
 */
// main function to perform inverse discrete fourier transform | 63
static int idft (
    const size_t nitems,
    const size_t stride,
    const trig_t * const trigs,
    const double complex * const xs,
    double complex * const ys
) {
  if (1 == nitems) {
    ys[0] = xs[0];
  } else if (0 == nitems % 2) {
    const size_t nitems_sub = nitems / 2;
    // divide-and-conquer
    idft(nitems_sub, stride * 2, trigs, xs + 0 * stride, ys + 0 * nitems_sub);
    idft(nitems_sub, stride * 2, trigs, xs + 1 * stride, ys + 1 * nitems_sub);
    // unify sub problem results
    for (size_t k = 0; k < nitems_sub; k++) {
      const trig_t * const trig_y1 = trigs + stride * 1 * k;
      const double complex c_y1 = trig_y1->cos + I * trig_y1->sin;
      double complex * const y0_ptr = ys + k + 0 * nitems_sub;
      double complex * const y1_ptr = ys + k + 1 * nitems_sub;
      const double complex y0 = *y0_ptr        / 2.;
      const double complex y1 = *y1_ptr * c_y1 / 2.;
      *y0_ptr = y0 + y1;
      *y1_ptr = y0 - y1;
    }
  } else if (0 == nitems % 3) {
    const size_t nitems_sub = nitems / 3;
    // divide-and-conquer
    idft(nitems_sub, stride * 3, trigs, xs + 0 * stride, ys + 0 * nitems_sub);
    idft(nitems_sub, stride * 3, trigs, xs + 1 * stride, ys + 1 * nitems_sub);
    idft(nitems_sub, stride * 3, trigs, xs + 2 * stride, ys + 2 * nitems_sub);
    // unify sub problem results
    for (size_t k = 0; k < nitems_sub; k++) {
      const trig_t * const trig_y1 = trigs + stride * 1 * k;
      const trig_t * const trig_y2 = trigs + stride * 2 * k;
      const double complex c_y1 = trig_y1->cos + I * trig_y1->sin;
      const double complex c_y2 = trig_y2->cos + I * trig_y2->sin;
      double complex * const y0_ptr = ys + k + 0 * nitems_sub;
      double complex * const y1_ptr = ys + k + 1 * nitems_sub;
      double complex * const y2_ptr = ys + k + 2 * nitems_sub;
      const double complex y0 = *y0_ptr        / 3.;
      const double complex y1 = *y1_ptr * c_y1 / 3.;
      const double complex y2 = *y2_ptr * c_y2 / 3.;
      const double complex m = - 0.5 - 0.8660254037844386 * I;
      const double complex p = - 0.5 + 0.8660254037844386 * I;
      *y0_ptr = y0 +     y1 +     y2;
      *y1_ptr = y0 + p * y1 + m * y2;
      *y2_ptr = y0 + m * y1 + p * y2;
    }
  } else {
    // naive O(N^2) iDFT
    for (size_t n = 0; n < nitems; n++) {
      double complex * const y = ys + n;
      *y = 0. + I * 0.;
      for (size_t k = 0; k < nitems; k++) {
        const size_t index = n * k % nitems;
        *y += xs[stride * k] * cexp(+ 2. * pi * index * I / nitems);
      }
      *y /= nitems;
    }
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
  double complex * const ys = plan->buf;
  dft(nitems, 1, plan->trigs, xs, ys);
  for (size_t i = 0; i < nitems; i++) {
    xs[i] = ys[i];
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
  idft(nitems, 1, plan->trigs, xs, ys);
  for (size_t i = 0; i < nitems; i++) {
    xs[i] = ys[i];
  }
  return 0;
}

int dft_init_plan (
    const size_t nitems,
    dft_plan_t ** const plan
) {
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

