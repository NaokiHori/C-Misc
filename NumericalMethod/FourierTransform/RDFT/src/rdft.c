#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "rdft.h"

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

/**
 * @brief Recursive radix-2 Cooley-Tukey FFT for complex input/output
 * @param nitems Degree of freedom of the given signal ("N" in the definition of complex DFT)
 * @param sign   Sign of the twiddle factor,
 *               -1 and +1 for forward and backward transforms, respectively
 * @param stride Distance between two successive input elements: 2^(level of recursion)
 * @param trigs  Pre-computed twiddle factors (trigonometric functions)
 * @param xs     Input  signal ordered as Re(0), Im(0), Re(1), Im(1), ...
 * @param ys     Output signal ordered as Re(0), Im(0), Re(1), Im(1), ...
 * @return success (0) or failure (1)
 */
static int dft (
    const size_t nitems,
    const double sign,
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
    dft(nh, sign, stride * 2, trigs, xs             , ys         );
    dft(nh, sign, stride * 2, trigs, xs + 2 * stride, ys + nitems);
    for (size_t i = 0; i < nh; i++) {
      const size_t j = i + nh;
      const trig_t * const trig = trigs + 2 * stride * i;
      const double twiddle[2] = {
        trig->cos,
        trig->sin * sign,
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
        const double arg = sign * 2. * pi * n * k / nitems;
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
  // from physical to spectral space | 1
  dft(nh, - 1., 1, trigs, xs, zs);
  // compute FFT of the original real signal, edge cases | 6
  {
    const double xe = zs[0];
    const double xo = zs[1];
    xs[ 0] = xe + xo;
    xs[nh] = xe - xo;
  }
  // compute FFT of the original real signal, common cases | 29
  for (size_t k = 1; k < nh; k++) {
    const double twiddle[2] = {
      + trigs[k].cos,
      - trigs[k].sin,
    };
    // Z_k
    const double z0[2] = {
      zs[2 * k + 0],
      zs[2 * k + 1],
    };
    // Z_{N / 2 - k}
    const double z1[2] = {
      zs[2 * (nh - k) + 0],
      zs[2 * (nh - k) + 1],
    };
    // X_k^e = + 1/2 Z_k + 1/2 Z_{N / 2 - k}^*
    const double xe[2] = {
      + 0.5 * z0[0] + 0.5 * z1[0],
      + 0.5 * z0[1] - 0.5 * z1[1],
    };
    // X_k^o = - I/2 Z_k + I/2 Z_{N / 2 - k}^*
    const double xo[2] = {
      + 0.5 * z0[1] + 0.5 * z1[1],
      - 0.5 * z0[0] + 0.5 * z1[0],
    };
    // real and imaginary parts are stored separately
    xs[         k] = xe[0] + xo[0] * twiddle[0] - xo[1] * twiddle[1];
    xs[nitems - k] = xe[1] + xo[1] * twiddle[0] + xo[0] * twiddle[1];
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
  const size_t nh = nitems / 2;
  const trig_t * const trigs = plan->trigs;
  double * const zs = plan->buf;
  // create a complex signal, edge cases | 6
  {
    const double x0 = xs[ 0];
    const double x1 = xs[nh];
    zs[0] = + 0.5 * x0 + 0.5 * x1;
    zs[1] = + 0.5 * x0 - 0.5 * x1;
  }
  // create a complex signal, common cases | 29
  for (size_t k = 1; k < nh; k++) {
    const double twiddle[2] = {
      trigs[k].cos,
      trigs[k].sin,
    };
    // X_k
    const double x0[2] = {
      + xs[         k],
      + xs[nitems - k],
    };
    // X_{N / 2 - k}
    const double x1[2] = {
      + xs[nh - k],
      + xs[nh + k],
    };
    // X_k^e = + 1/2 X_k + 1/2 X_{N / 2 - k}^*
    const double xe[2] = {
      + 0.5 * x0[0] + 0.5 * x1[0],
      + 0.5 * x0[1] - 0.5 * x1[1],
    };
    // X_k^o = + I/2 X_k - I/2 X_{N / 2 - k}^*
    const double xo[2] = {
      - 0.5 * x0[1] - 0.5 * x1[1],
      + 0.5 * x0[0] - 0.5 * x1[0],
    };
    // real and imaginary parts are stored alternately
    zs[2 * k + 0] = xe[0] + xo[0] * twiddle[0] - xo[1] * twiddle[1];
    zs[2 * k + 1] = xe[1] + xo[1] * twiddle[0] + xo[0] * twiddle[1];
  }
  // from spectral to physical space | 1
  dft(nh, + 1., 1, trigs, zs, xs);
  // NOTE: performing DFTs whose size is nitems / 2
  //       halves the amplitude of the resulting signal,
  //       which is compensated here
  for (size_t i = 0; i < nitems; i++) {
    xs[i] *= 2.;
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

