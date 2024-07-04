#include <stdlib.h>
#include <complex.h>
#include <fftw3.h>
#include "interp_spectral.h"

// FFT-based Interpolation for uniform grids.
// For example, I consider a dataset y1s, whose spectral representation is
//                 z4+z4*
// z0, z1, z2, z3, ------, z3*, z2*, z1*
//                   2
// This is interpolated in the spectral space as
//                 z4           z4*
// z0, z1, z2, z3, --, 0, 0, 0, ---, z3*, z2*, z1*
//                  2            2
// Note that the asterisks denote the complex conjugate,
//   and above z4 = z4* is fulfilled since y1s is a real vector.
// Since FFTW stores only the first half part,
//   the above procedure is much simplified in practice below.

struct interp_spectral_plan_t {
  size_t n1;
  size_t n2;
  double * y1s_;
  double * y2s_;
  fftw_complex * z1s_;
  fftw_complex * z2s_;
  fftw_plan plans[2];
};

static void * memory_alloc (
    const size_t nitems,
    const size_t size
) {
  void * ptr = malloc(nitems * size);
  if (NULL == ptr) {
    fprintf(stderr, "FATAL: memory allocation error\n");
    exit(EXIT_FAILURE);
  }
  return ptr;
}

static void memory_free (
    void * ptr
) {
  free(ptr);
}

int interp_spectral_initialise (
    const size_t n1,
    const size_t n2,
    interp_spectral_plan_t ** plan
) {
  // allocate main plan
  *plan = memory_alloc(1, sizeof(interp_spectral_plan_t));
  // vector lengths in spectral space
  const size_t n1_s = n1 / 2 + 1;
  const size_t n2_s = n2 / 2 + 1;
  (*plan)->n1 = n1;
  (*plan)->n2 = n2;
  // buffers in the physical space for internal use
  double ** y1s_ = &(*plan)->y1s_;
  double ** y2s_ = &(*plan)->y2s_;
  // buffers in the ospectral space for internal use
  fftw_complex ** z1s_ = &(*plan)->z1s_;
  fftw_complex ** z2s_ = &(*plan)->z2s_;
  // fftw plans
  fftw_plan * plans = (*plan)->plans;
  // allocate buffers
  *y1s_ = memory_alloc(n1, sizeof(double));
  *y2s_ = memory_alloc(n2, sizeof(double));
  *z1s_ = memory_alloc(n1_s, sizeof(fftw_complex));
  *z2s_ = memory_alloc(n2_s, sizeof(fftw_complex));
  // create plans
  plans[0] = fftw_plan_dft_r2c_1d(n1, *y1s_, *z1s_, FFTW_MEASURE);
  plans[1] = fftw_plan_dft_c2r_1d(n2, *z2s_, *y2s_, FFTW_MEASURE);
  if (NULL == plans[0] || NULL == plans[1]) {
    goto abort;
  }
  return 0;
abort:
  if (plans[0]) {
    fftw_destroy_plan(plans[0]);
  }
  if (plans[1]) {
    fftw_destroy_plan(plans[1]);
  }
  memory_free(*y1s_);
  memory_free(*y2s_);
  memory_free(*z1s_);
  memory_free(*z2s_);
  memory_free(*plan);
  return 1;
}

int interp_spectral_execute (
    interp_spectral_plan_t * plan,
    const double * y1s,
    double * y2s
) {
  const size_t n1 = plan->n1;
  const size_t n2 = plan->n2;
  const size_t n1_s = n1 / 2 + 1;
  const size_t n2_s = n2 / 2 + 1;
  double * y1s_ = plan->y1s_;
  double * y2s_ = plan->y2s_;
  fftw_complex * z1s_ = plan->z1s_;
  fftw_complex * z2s_ = plan->z2s_;
  fftw_plan * plans = plan->plans;
  // copy and normalise
  for (size_t i = 0; i < n1; i++) {
    y1s_[i] = y1s[i] / n1;
  }
  // project original physical vector to spectral space
  fftw_execute(plans[0]);
  // assign positive wavenumbers
  for (size_t i = 0; i < n1_s - 1; i++) {
    z2s_[i] = z1s_[i];
  }
  for (size_t i = n1_s - 1; i < n2_s; i++) {
    z2s_[i] = 0.;
  }
  // project refined vector to physical space
  fftw_execute(plans[1]);
  // copy to user buffer
  for (size_t i = 0; i < n2; i++) {
    y2s[i] = y2s_[i];
  }
  return 0;
}

int interp_spectral_finalise (
    interp_spectral_plan_t ** plan
) {
  fftw_plan * fftw_plans = (*plan)->plans;
  // clean-up fftw plans
  if (fftw_plans[0]) {
    fftw_destroy_plan(fftw_plans[0]);
  }
  if (fftw_plans[1]) {
    fftw_destroy_plan(fftw_plans[1]);
  }
  // deallocate buffers
  memory_free((*plan)->y1s_);
  memory_free((*plan)->y2s_);
  memory_free((*plan)->z1s_);
  memory_free((*plan)->z2s_);
  memory_free(*plan);
  *plan = NULL;
  return 0;
}

