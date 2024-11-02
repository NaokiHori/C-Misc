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
// main function to perform discrete fourier transform | 38
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
    const size_t nh = nitems / 2;
    // divide-and-conquer
    dft(nh, stride * 2, trigs, xs         , ys     );
    dft(nh, stride * 2, trigs, xs + stride, ys + nh);
    // unify two sub problem results
    for (size_t i = 0; i < nh; i++) {
      const size_t j = i + nh;
      const trig_t * const trig = trigs + stride * i;
      const double complex twiddle = trig->cos - I * trig->sin;
      double complex * const yi = ys + i;
      double complex * const yj = ys + j;
      const double complex ye = *yi;
      const double complex yo = *yj * twiddle;
      *yi = ye + yo;
      *yj = ye - yo;
    }
  } else {
    // naive O(N^2) DFT
    for (size_t k = 0; k < nitems; k++) {
      double complex * const y = ys + k;
      *y = 0. + I * 0.;
      for (size_t n = 0; n < nitems; n++) {
        *y += xs[stride * n] * cexp(- 2. * pi * n * k * I / nitems);
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
// main function to perform inverse discrete fourier transform | 39
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
    const size_t nh = nitems / 2;
    // divide-and-conquer
    idft(nh, stride * 2, trigs, xs         , ys     );
    idft(nh, stride * 2, trigs, xs + stride, ys + nh);
    // unify two sub problem results
    for (size_t i = 0; i < nh; i++) {
      const size_t j = i + nh;
      const trig_t * const trig = trigs + stride * i;
      const double complex twiddle = trig->cos + I * trig->sin;
      double complex * const yi = ys + i;
      double complex * const yj = ys + j;
      const double complex ye = 0.5 * *yi;
      const double complex yo = 0.5 * *yj * twiddle;
      *yi = ye + yo;
      *yj = ye - yo;
    }
  } else {
    // naive O(N^2) iDFT
    for (size_t n = 0; n < nitems; n++) {
      double complex * const y = ys + n;
      *y = 0. + I * 0.;
      for (size_t k = 0; k < nitems; k++) {
        *y += xs[stride * k] * cexp(+ 2. * pi * n * k * I / nitems);
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

