#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include "mydct.h"

static const double SQRT2  = 1.4142135623730951;
// sqrt(3) / 2
static const double SQRT3H = 0.8660254037844386;

struct mydct_plan_t {
  // size of the input / output signals
  size_t nitems;
  // recover original order or not
  bool reorder;
  // internal buffers in which
  //   real and imag numbers alternate,
  //   i.e. r[0] i[0] r[1] i[1] ...
  double * restrict cxs;
  double * restrict cys;
  // trigonometric tables
  //   "2 pi i / N"
  //   and
  //   "pi i / 2 / N",
  //   where i = 0, 1, ..., N/2-1
  // 0: cos, 1: sin
  double (* restrict mult1s)[2];
  double (* restrict mult2s)[2];
};

static void * memory_alloc (
    const size_t size
) {
  void * ptr = malloc(size);
  if (NULL == ptr) {
    fprintf(stderr, "[mydct FATAL] failed to allocate %zu bytes\n", size);
    return NULL;
  }
  return ptr;
}

static void memory_free (
    void * ptr
) {
  free(ptr);
}

static bool is_valid_size (
    size_t nitems
) {
  const char message[] = "nitems should be 2 ^ (N + 2) or 3 x 2 ^ (N + 2)";
  if (nitems < 4) {
    fprintf(stderr, message);
    return false;
  }
  if (0 == nitems % 3) {
    nitems /= 3;
  }
  if (nitems & (nitems - 1)) {
    fprintf(stderr, message);
    return false;
  }
  return true;
}

// compute discrete Fourier transform, using Cooley-Tukey algorithm
static inline int fft (
    const bool is_forward,
    const size_t nitems,
    const size_t stride,
    const double (* restrict mults)[2],
    const double * restrict xs,
    double * restrict ys
) {
  // small sizes, use analytical solutions
  if (1 == nitems) {
    ys[0] = xs[0];
    ys[1] = xs[1];
    return 0;
  }
  if (2 == nitems) {
    const double x00 = xs[0 * stride * 2 + 0];
    const double x01 = xs[0 * stride * 2 + 1];
    const double x10 = xs[1 * stride * 2 + 0];
    const double x11 = xs[1 * stride * 2 + 1];
    ys[0] = x00 + x10;
    ys[1] = x01 + x11;
    ys[2] = x00 - x10;
    ys[3] = x01 - x11;
    return 0;
  }
  const double sign = is_forward ? -1. : +1.;
  if (3 == nitems) {
    const double x00 = xs[0 * stride * 2 + 0];
    const double x01 = xs[0 * stride * 2 + 1];
    const double x10 = xs[1 * stride * 2 + 0];
    const double x11 = xs[1 * stride * 2 + 1];
    const double x20 = xs[2 * stride * 2 + 0];
    const double x21 = xs[2 * stride * 2 + 1];
    ys[0] = x00 + x10 + x20;
    ys[1] = x01 + x11 + x21;
    ys[2] = (x00 - 0.5 * x10 - 0.5 * x20) - sign * SQRT3H * (x11 - x21);
    ys[3] = (x01 - 0.5 * x11 - 0.5 * x21) + sign * SQRT3H * (x10 - x20);
    ys[4] = (x00 - 0.5 * x10 - 0.5 * x20) + sign * SQRT3H * (x11 - x21);
    ys[5] = (x01 - 0.5 * x11 - 0.5 * x21) - sign * SQRT3H * (x10 - x20);
    return 0;
  }
  if (4 == nitems) {
    const double x00 = xs[0 * stride * 2 + 0];
    const double x01 = xs[0 * stride * 2 + 1];
    const double x10 = xs[1 * stride * 2 + 0];
    const double x11 = xs[1 * stride * 2 + 1];
    const double x20 = xs[2 * stride * 2 + 0];
    const double x21 = xs[2 * stride * 2 + 1];
    const double x30 = xs[3 * stride * 2 + 0];
    const double x31 = xs[3 * stride * 2 + 1];
    ys[0] = (x00 + x20) +        (x10 + x30);
    ys[1] = (x01 + x21) +        (x11 + x31);
    ys[2] = (x00 - x20) - sign * (x11 - x31);
    ys[3] = (x01 - x21) + sign * (x10 - x30);
    ys[4] = (x00 + x20) -        (x10 + x30);
    ys[5] = (x01 + x21) -        (x11 + x31);
    ys[6] = (x00 - x20) + sign * (x11 - x31);
    ys[7] = (x01 - x21) - sign * (x10 - x30);
    return 0;
  }
  // general 2^N sizes, use recurrent relations
  // an FFT is divided into two sub-FFTs: even-indexed and odd-indexed
  // e.g., N = 8
  //   even-indexed
  //   x[0], x[2], x[4], x[6], ... -> y[0], y[1], y[2], y[3], ...
  //   odd -indexed
  //   x[1], x[3], x[5], x[7], ... -> y[4], y[5], y[6], y[7], ...
  fft(is_forward, nitems / 2, stride * 2, mults, xs,              ys         );
  fft(is_forward, nitems / 2, stride * 2, mults, xs + stride * 2, ys + nitems);
  // combine even and odd results
  for (size_t i = 0; i < nitems / 2; i++) {
    const size_t j = i + nitems / 2;
    // multiplier, equivalent to
    //   mult_c = cos( 2 * pi * i / nitems )
    //   mult_s = sin( 2 * pi * i / nitems )
    const double mult_c = mults[stride * 2 * i][0];
    const double mult_s = mults[stride * 2 * i][1];
    // exchange between i and j
    const double i0 = ys[i * 2 + 0];
    const double i1 = ys[i * 2 + 1];
    const double j0 = ys[j * 2 + 0];
    const double j1 = ys[j * 2 + 1];
    ys[i * 2 + 0] = i0 + j0 * mult_c - sign * j1 * mult_s;
    ys[i * 2 + 1] = i1 + j1 * mult_c + sign * j0 * mult_s;
    ys[j * 2 + 0] = i0 - j0 * mult_c + sign * j1 * mult_s;
    ys[j * 2 + 1] = i1 - j1 * mult_c - sign * j0 * mult_s;
  }
  return 0;
}

int mydct_init_plan (
    const size_t nitems,
    const bool reorder,
    mydct_plan_t ** plan
) {
  if (!is_valid_size(nitems)) {
    return 1;
  }
  // allocate, initalise, and pack
  (*plan) = memory_alloc(1 * sizeof(mydct_plan_t));
  if (NULL == *plan) {
    return 1;
  }
  (*plan)->nitems = nitems;
  (*plan)->reorder = reorder;
  (*plan)->cxs = memory_alloc((nitems + 2) * sizeof(double));
  (*plan)->cys = memory_alloc((nitems + 2) * sizeof(double));
  if (NULL == (*plan)->cxs || NULL == (*plan)->cys) {
    return 1;
  }
  double (* restrict * mult1s)[2] = &(*plan)->mult1s;
  double (* restrict * mult2s)[2] = &(*plan)->mult2s;
  *mult1s = memory_alloc(nitems / 2 * sizeof(double [2]));
  *mult2s = memory_alloc(nitems / 2 * sizeof(double [2]));
  if (NULL == (*plan)->mult1s || NULL == (*plan)->mult2s) {
    return 1;
  }
  const double pi = 3.141592653589793238462643383;
  for (size_t i = 0; i < nitems / 2; i++) {
    const double phase = 2. * pi * i / nitems;
    (*mult1s)[i][0] = cos(phase);
    (*mult1s)[i][1] = sin(phase);
  }
  for (size_t i = 0; i < nitems / 2; i++) {
    const double phase = pi * i / 2. / nitems;
    (*mult2s)[i][0] = cos(phase);
    (*mult2s)[i][1] = sin(phase);
  }
  return 0;
}

int mydct_destroy_plan (
    mydct_plan_t ** plan
) {
  if (NULL == *plan) {
    fprintf(stderr, "the plan is NULL\n");
    return 1;
  }
  memory_free((*plan)->cxs);
  memory_free((*plan)->cys);
  memory_free((*plan)->mult1s);
  memory_free((*plan)->mult2s);
  memory_free(*plan);
  *plan = NULL;
  return 0;
}

static inline int exchange (
    const bool is_forward,
    const double * restrict mult,
    double * restrict i,
    double * restrict j
) {
  // exchange information between i and j, fig. 5
  const double sign = is_forward ? -1. : +1.;
  const double i0 = *(i + 0);
  const double i1 = *(i + 1);
  const double j0 = *(j + 0);
  const double j1 = *(j + 1);
  const double a0 = + i0 + j0;
  const double s0 = + i0 - j0;
  const double a1 = + i1 + j1;
  const double s1 = + i1 - j1;
  *(i + 0) = + a0 - sign * mult[0] * a1 - mult[1] * s0;
  *(i + 1) = + s1 + sign * mult[0] * s0 - mult[1] * a1;
  *(j + 0) = + a0 + sign * mult[0] * a1 + mult[1] * s0;
  *(j + 1) = - s1 + sign * mult[0] * s0 - mult[1] * a1;
  return 0;
}

// fast cosine transform by Makhoul 1980
int mydct_exec_f (
    mydct_plan_t * plan,
    const double * restrict xs,
    double * restrict ys
){
  if (NULL == plan) {
    fprintf(stderr, "the plan is NULL\n");
    return 1;
  }
  const bool is_forward = true;
  const size_t nitems = plan->nitems;
  const bool reorder = plan->reorder;
  double * restrict cxs = plan->cxs;
  double * restrict cys = plan->cys;
  const double (* restrict mult1s)[2] = plan->mult1s;
  const double (* restrict mult2s)[2] = plan->mult2s;
  // step 1: eq. 20 and eq. A-1 are combined
  if (reorder) {
    for (size_t i = 0; i < nitems / 4; i++) {
      const size_t j = nitems / 2 - i - 1;
      cxs[i * 2 + 0] = xs[4 * i + 0];
      cxs[i * 2 + 1] = xs[4 * i + 2];
      cxs[j * 2 + 0] = xs[4 * i + 3];
      cxs[j * 2 + 1] = xs[4 * i + 1];
    }
  }
  // step 2: fft, followed by fig. 5a
  fft(is_forward, nitems / 2, 1, mult1s, reorder ? cxs : xs, cys);
  ys[         0] = 2.    * (cys[0] + cys[1]);
  ys[nitems / 2] = SQRT2 * (cys[0] - cys[1]);
  cys[nitems / 2 + 0] *= + 2.;
  cys[nitems / 2 + 1] *= - 2.;
  for (size_t i = 1; i < nitems / 4; i++) {
    const size_t j = nitems / 2 - i;
    exchange(is_forward, mult1s[i], cys + 2 * i, cys + 2 * j);
  }
  // step 3
  for (size_t i = 1; i < nitems / 2; i++) {
    const size_t j = nitems - i;
    const double mult_c = mult2s[i][0];
    const double mult_s = mult2s[i][1];
    ys[i] = + cys[i * 2 + 0] * mult_c + cys[i * 2 + 1] * mult_s;
    ys[j] = - cys[i * 2 + 1] * mult_c + cys[i * 2 + 0] * mult_s;
  }
  return 0;
}

// inverse fast cosine transform by Makhoul 1980
// NOTE: the results are doubled so that it is equivalent to FFTW IDCT
int mydct_exec_b (
    mydct_plan_t * plan,
    const double * restrict xs,
    double * restrict ys
) {
  if (NULL == plan) {
    fprintf(stderr, "the plan is NULL\n");
    return 1;
  }
  const bool is_forward = false;
  const size_t nitems = plan->nitems;
  const bool reorder = plan->reorder;
  double * restrict cxs = plan->cxs;
  double * restrict cys = plan->cys;
  const double (* restrict mult1s)[2] = plan->mult1s;
  const double (* restrict mult2s)[2] = plan->mult2s;
  // step 1: eq. 28
  for (size_t i = 1; i < nitems / 2; i++) {
    const size_t j = nitems - i;
    const double mult_c = mult2s[i][0];
    const double mult_s = mult2s[i][1];
    cxs[i * 2 + 0] = xs[i] * mult_c + xs[j] * mult_s;
    cxs[i * 2 + 1] = xs[i] * mult_s - xs[j] * mult_c;
  }
  // step 2: fig. 5(b), followed by iFFT
  cxs[1] = xs[0] - SQRT2 * xs[nitems / 2];
  cxs[0] = xs[0] + SQRT2 * xs[nitems / 2];
  cxs[nitems / 2 + 0] *= + 2.;
  cxs[nitems / 2 + 1] *= - 2.;
  for (size_t i = 1; i < nitems / 4; i++) {
    const size_t j = nitems / 2 - i;
    exchange(is_forward, mult1s[i], cxs + 2 * i, cxs + 2 * j);
  }
  fft(is_forward, nitems / 2, 1, mult1s, cxs, reorder ? cys : ys);
  // step 3: eq. 20 and eq. A-1 are combined
  if (reorder) {
    for (size_t i = 0; i < nitems / 4; i++) {
      const size_t j = nitems / 2 - i - 1;
      ys[4 * i + 0] = cys[i * 2 + 0];
      ys[4 * i + 2] = cys[i * 2 + 1];
      ys[4 * i + 3] = cys[j * 2 + 0];
      ys[4 * i + 1] = cys[j * 2 + 1];
    }
  }
  return 0;
}

