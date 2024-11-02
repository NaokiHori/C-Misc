#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <complex.h>
#include "sdft.h"

static const double pi = 3.14159265358979324;

typedef struct {
  double cos;
  double sin;
} trig_t;

struct sdft_plan_t {
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
 * @param nitems Degree of freedom of the given signal ("N" in the definition of complex sdft)
 * @param stride Distance between two successive input elements: 2^(level of recursion)
 * @param trigs  Pre-computed twiddle factors (trigonometric functions)
 * @param xs     Input  signal ordered as Re(0), Im(0), Re(1), Im(1), ...
 * @param ys     Output signal ordered as Re(0), Im(0), Re(1), Im(1), ...
 * @return success (0) or failure (1)
 */
// main function to perform discrete fourier transform | 41
static int sdft (
    const size_t nitems,
    const size_t stride,
    const trig_t * const trigs,
    const double complex * const xs,
    double complex * const ys
) {
  if (1 == nitems) {
    ys[0] = xs[0];
  } else if (0 == nitems % 2) {
    const size_t nh = nitems / 2;
    // divide-and-conquer
    sdft(nh, stride * 2, trigs, xs         , ys     );
    sdft(nh, stride * 2, trigs, xs + stride, ys + nh);
    // unify two sub problem results
    for (size_t i = 0; i < nh; i++) {
      const size_t j = i + nh;
      const trig_t * const trig = trigs + stride * i;
      const double complex twiddle[2] = {
        trig->cos + I * trig->sin,
        trig->cos - I * trig->sin,
      };
      double complex * const yi = ys + i;
      double complex * const yj = ys + j;
      const double complex ye = *yi;
      const double complex yo = *yj;
      *yi =       twiddle[0] * ye +     twiddle[1] * yo;
      *yj = - I * twiddle[0] * ye + I * twiddle[1] * yo;
    }
  } else {
    // naive O(N^2) sdft
    for (size_t k = 0; k < nitems; k++) {
      double complex * const y = ys + k;
      *y = 0. + I * 0.;
      for (size_t n = 0; n < nitems; n++) {
        *y += xs[stride * n] * cexp(- 2. * pi * (n + 0.5) * k * I / nitems);
      }
    }
  }
  return 0;
}

/**
 * @brief Recursive radix-2 Cooley-Tukey FFT for complex input/output, backward transform
 * @param nitems Degree of freedom of the given signal ("N" in the definition of complex sdft)
 * @param stride Distance between two successive input elements: 2^(level of recursion)
 * @param trigs  Pre-computed twiddle factors (trigonometric functions)
 * @param xs     Input  signal ordered as Re(0), Im(0), Re(1), Im(1), ...
 * @param ys     Output signal ordered as Re(0), Im(0), Re(1), Im(1), ...
 * @return success (0) or failure (1)
 */
// main function to perform inverse discrete fourier transform | 39
static int isdft (
    const size_t nitems,
    const size_t stride,
    const trig_t * const trigs,
    const double complex * const xs,
    double complex * const ys
) {
  if (1 == nitems) {
    ys[0] = xs[0];
  } else if (0 == nitems % 2) {
    const size_t nh = nitems / 2;
    // divide-and-conquer
    isdft(nh, stride * 2, trigs, xs         , ys     );
    isdft(nh, stride * 2, trigs, xs + stride, ys + nh);
    // unify two sub problem results
    for (size_t i = 0; i < nh; i++) {
      const size_t j = i + nh;
      const trig_t * const trig = trigs + (2 * i + 1) * stride;
      const double complex twiddle = trig->cos + I * trig->sin;
      double complex * const yi = ys + i;
      double complex * const yj = ys + j;
      const double complex ye = *yi;
      const double complex yo = *yj;
      *yi = 0.5 * ye + 0.5 * twiddle * yo;
      *yj = 0.5 * ye - 0.5 * twiddle * yo;
    }
  } else {
    // naive O(N^2) isdft
    for (size_t n = 0; n < nitems; n++) {
      double complex * const y = ys + n;
      *y = 0. + I * 0.;
      for (size_t k = 0; k < nitems; k++) {
        *y += xs[stride * k] * cexp(2. * pi * (n + 0.5) * k * I / nitems);
      }
      *y /= nitems;
    }
  }
  return 0;
}

int sdft_exec_f (
    sdft_plan_t * const plan,
    double complex * const xs
) {
  if (NULL == plan) {
    puts("uninitialized plan is passed");
    return 1;
  }
  const size_t nitems = plan->nitems;
  double complex * const ys = plan->buf;
  sdft(nitems, 1, plan->trigs, xs, ys);
  for (size_t i = 0; i < nitems; i++) {
    xs[i] = ys[i];
  }
  return 0;
}

int sdft_exec_b (
    sdft_plan_t * const plan,
    double complex * const xs
) {
  if (NULL == plan) {
    puts("uninitialized plan is passed");
    return 1;
  }
  const size_t nitems = plan->nitems;
  double complex * const ys = plan->buf;
  isdft(nitems, 1, plan->trigs, xs, ys);
  for (size_t i = 0; i < nitems; i++) {
    xs[i] = ys[i];
  }
  return 0;
}

int sdft_init_plan (
    const size_t nitems,
    sdft_plan_t ** const plan
) {
  *plan = memory_alloc(1 * sizeof(sdft_plan_t));
  (*plan)->nitems = nitems;
  trig_t ** const trigs = &(*plan)->trigs;
  double complex ** const buf = &(*plan)->buf;
  *trigs = memory_alloc((2 * nitems) * sizeof(trig_t));
  *buf = memory_alloc(nitems * sizeof(double complex));
  // prepare cosine / sine tables
  for (size_t i = 0; i < 2 * nitems; i++) {
    trig_t * const trig = *trigs + i;
    trig->cos = cos(pi * i / nitems);
    trig->sin = sin(pi * i / nitems);
  }
  return 0;
}

// clean-up a plan
int sdft_destroy_plan (
    sdft_plan_t ** const plan
) {
  memory_free((*plan)->trigs);
  memory_free((*plan)->buf);
  memory_free(*plan);
  *plan = NULL;
  return 0;
}

