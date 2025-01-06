#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <complex.h>
#include "rdft.h"

static const double pi = 3.14159265358979324;

typedef struct {
  double cos;
  double sin;
} trig_t;

typedef struct {
  double real;
  double imag;
} complex_t;

struct rdft_plan_t {
  // degree of freedom
  size_t nitems;
  // pre-computed trigonometric values
  trig_t * trigs;
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

static int dft (
    const size_t nitems,
    const size_t stride,
    const trig_t * const trigs,
    const double * const xs,
    double * const ys
) {
  if (1 == nitems) {
    ys[0] = xs[0];
    ys[1] = xs[1];
  } else if (0 == nitems % 2) {
    const size_t nh = nitems / 2;
    dft(nh, stride * 2, trigs, xs             , ys         );
    dft(nh, stride * 2, trigs, xs + 2 * stride, ys + nitems);
    for (size_t i = 0; i < nh; i++) {
      const size_t j = i + nh;
      const trig_t * const trig = trigs + 2 * stride * i;
      const double twiddle[2] = {
        + 1. * trig->cos,
        - 1. * trig->sin,
      };
      double * const yi = ys + 2 * i;
      double * const yj = ys + 2 * j;
      const double e[2] = {
        yi[0],
        yi[1],
      };
      const double o[2] = {
        yj[0] * twiddle[0] - yj[1] * twiddle[1],
        yj[1] * twiddle[0] + yj[0] * twiddle[1],
      };
      yi[0] = e[0] + o[0];
      yi[1] = e[1] + o[1];
      yj[0] = e[0] - o[0];
      yj[1] = e[1] - o[1];
    }
  } else {
    // naive O(N^2) DFT
    for (size_t k = 0; k < nitems; k++) {
      double * const y = ys + 2 * k;
      y[0] = 0.;
      y[1] = 0.;
      for (size_t n = 0; n < nitems; n++) {
        const double arg = - 2. * pi * n * k / nitems;
        const double c = cos(arg);
        const double s = sin(arg);
        const double * const x = xs + 2 * stride * n;
        y[0] += x[0] * c - x[1] * s;
        y[1] += x[1] * c + x[0] * s;
      }
    }
  }
  return 0;
}

static int idft (
    const size_t nitems,
    const size_t stride,
    const trig_t * const trigs,
    const double * const xs,
    double * const ys
) {
  if (1 == nitems) {
    ys[0] = xs[0];
    ys[1] = xs[1];
  } else if (0 == nitems % 2) {
    const size_t nh = nitems / 2;
    idft(nh, stride * 2, trigs, xs             , ys         );
    idft(nh, stride * 2, trigs, xs + 2 * stride, ys + nitems);
    for (size_t i = 0; i < nh; i++) {
      const size_t j = i + nh;
      const trig_t * const trig = trigs + 2 * stride * i;
      const double twiddle[2] = {
        + 1. * trig->cos,
        + 1. * trig->sin,
      };
      double * const yi = ys + 2 * i;
      double * const yj = ys + 2 * j;
      const double e[2] = {
        yi[0],
        yi[1],
      };
      const double o[2] = {
        yj[0] * twiddle[0] - yj[1] * twiddle[1],
        yj[1] * twiddle[0] + yj[0] * twiddle[1],
      };
      yi[0] = 0.5 * e[0] + 0.5 * o[0];
      yi[1] = 0.5 * e[1] + 0.5 * o[1];
      yj[0] = 0.5 * e[0] - 0.5 * o[0];
      yj[1] = 0.5 * e[1] - 0.5 * o[1];
    }
  } else {
    // naive O(N^2) iDFT
    for (size_t k = 0; k < nitems; k++) {
      double * const y = ys + 2 * k;
      y[0] = 0.;
      y[1] = 0.;
      for (size_t n = 0; n < nitems; n++) {
        const double arg = + 2. * pi * n * k / nitems;
        const double c = cos(arg);
        const double s = sin(arg);
        const double * const x = xs + 2 * stride * n;
        y[0] += x[0] * c - x[1] * s;
        y[1] += x[1] * c + x[0] * s;
      }
      y[0] /= nitems;
      y[1] /= nitems;
    }
  }
  return 0;
}

// compute forward transform | 56
int rdft_exec_f (
    rdft_plan_t * const plan,
    double * const xs
) {
  if (NULL == plan) {
    puts("uninitialized plan is passed");
    return 1;
  }
  const size_t nitems = plan->nitems;
  const size_t nh = nitems / 2;
  const trig_t * const trigs = plan->trigs;
  double * const zs = plan->buf;
  // compute complex dft to find Z_k | 1
  dft(nh, 1, trigs, xs, zs);
  // compute FFT of the original real signal, corner cases | 6
  {
    const double xe = zs[0];
    const double xo = zs[1];
    xs[ 0] = xe + xo;
    xs[nh] = xe - xo;
  }
  // compute FFT of the original real signal, common cases
  for (size_t k = 1; k < nh; k++) {
    // trigonometric table stores
    //   cos(+ 2 pi k / nitems)
    //   +
    //   sin(+ 2 pi k / nitems) I
    // here we need
    //   cos(- 2 pi k / nitems)
    //   +
    //   sin(- 2 pi k / nitems) I
    const double complex twiddle =
      + 1. * trigs[k].cos
      - 1. * trigs[k].sin * I;
    // compute 1/2 Z_k | 3
    const double complex z0 =
      + 0.5 * zs[2 * k + 0]
      + 0.5 * zs[2 * k + 1] * I;
    // compute 1/2 Z_{N / 2 - k} | 3
    const double complex z1 =
      + 0.5 * zs[2 * (nh - k) + 0]
      + 0.5 * zs[2 * (nh - k) + 1] * I;
    // compute X_k^e = + 1/2 Z_k + 1/2 Z_{N / 2 - k}^* | 1
    const double complex xe = +     z0 +     conj(z1);
    // compute X_k^o = - I/2 Z_k + I/2 Z_{N / 2 - k}^* | 1
    const double complex xo = - I * z0 + I * conj(z1);
    // real and imaginary parts are stored separately | 6
    // X_k^e + exp(- 2 pi k / nitems) X_k^o
    // X_k^e - exp(- 2 pi k / nitems) X_k^o
    const double complex za = xe + xo * twiddle;
    const double complex zb = xe - xo * twiddle;
    xs[k     ] = creal(za);
    xs[k + nh] = cimag(zb);
  }
  return 0;
}

// compute backward transform | 52
int rdft_exec_b (
    rdft_plan_t * const plan,
    double * const xs
) {
  if (NULL == plan) {
    puts("uninitialized plan is passed");
    return 1;
  }
  const size_t nitems = plan->nitems;
  const size_t nh = nitems / 2;
  const trig_t * const trigs = plan->trigs;
  double * const zs = plan->buf;
  // create a complex signal, edge cases | 9
  {
    const double x0 = 0.5 * xs[ 0];
    const double x1 = 0.5 * xs[nh];
    const double complex xe = x0 + x1;
    const double complex xo = x0 - x1;
    const double complex z = xe + xo * I;
    zs[0] = creal(z);
    zs[1] = cimag(z);
  }
  // create a complex signal, common cases
  for (size_t k = 1; k < nh; k++) {
    // trigonometric table
    //   cos(+ 2 pi k / nitems)
    //   sin(+ 2 pi k / nitems)
    const double complex twiddle =
      + 1. * trigs[k].cos
      + 1. * trigs[k].sin * I;
    // compute 1/2 X_k | 3
    const double complex x0 =
      + 0.5 * xs[         k]
      - 0.5 * xs[nitems - k] * I;
    // compute 1/2 X_{k + N / 2} | 3
    const double complex x1 =
      + 0.5 * xs[nh - k]
      + 0.5 * xs[nh + k] * I;
    // X_k^e = (+ 1/2 X_k + 1/2 X_{k + N / 2}) | 1
    const double complex xe = (x0 + x1);
    // X_k^o = (+ 1/2 X_k - 1/2 X_{k + N / 2}) * exp(arg) | 1
    const double complex xo = (x0 - x1) * twiddle;
    // Z_k = X_k^e + X_k^o * I | 4
    // real and imaginary parts are stored alternately
    const double complex z = xe + xo * I;
    zs[2 * k + 0] = creal(z);
    zs[2 * k + 1] = cimag(z);
  }
  // compute complex idft to find z_n | 1
  idft(nh, 1, trigs, zs, xs);
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
  trig_t ** const trigs = &(*plan)->trigs;
  double ** const buf = &(*plan)->buf;
  const size_t nh = nitems / 2;
  *trigs = memory_alloc(nh * sizeof(trig_t));
  *buf = memory_alloc(nitems * sizeof(double));
  // prepare trigonometric tables
  for (size_t i = 0; i < nh; i++) {
    trig_t * const trig = *trigs + i;
    trig->cos = cos(2. * pi * i / nitems);
    trig->sin = sin(2. * pi * i / nitems);
  }
  return 0;
}

// clean-up a plan
int rdft_destroy_plan (
    rdft_plan_t ** const plan
) {
  memory_free((*plan)->trigs);
  memory_free((*plan)->buf);
  memory_free(*plan);
  *plan = NULL;
  return 0;
}

