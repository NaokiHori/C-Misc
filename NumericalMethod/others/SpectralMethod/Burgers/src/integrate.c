#include <stdlib.h>
#include <complex.h>
#include "integrate.h"

static int compute_adv_fft_div (
    const int nitems,
    const double dt,
    my_ffts_t * const my_ffts,
    const fftw_complex * const gs,
    fftw_complex * const dgs
) {
  double * const fs = calloc(nitems, sizeof(double));
  fftw_complex * const hs = calloc(nitems / 2 + 1, sizeof(fftw_complex));
  // go to physical space
  my_fft_c2r(my_ffts, gs, fs);
  // compute product in physical space
  for (int i = 0; i < nitems; i++) {
    fs[i] *= fs[i];
  }
  // go back to spectral space
  my_fft_r2c(my_ffts, fs, hs);
  // derivative
  for (int k = 0; k < nitems / 2 + 1; k++) {
    hs[k] *= 0.5 * I * k;
  }
  // normalise
  for (int k = 0; k < nitems / 2 + 1; k++) {
    hs[k] /= nitems;
  }
  // obtain increment
  for (int k = 0; k < nitems / 2 + 1; k++) {
    dgs[k] -= dt * hs[k];
  }
  free(fs);
  free(hs);
  return 0;
}

static int compute_adv_fft_adv (
    const int nitems,
    const double dt,
    my_ffts_t * const my_ffts,
    const fftw_complex * const gs,
    fftw_complex * const dgs
) {
  fftw_complex * const hs = calloc(nitems / 2 + 1, sizeof(fftw_complex));
  double * const es = calloc(nitems, sizeof(double));
  double * const fs = calloc(nitems, sizeof(double));
  // compute derivative
  for (int k = 0; k < nitems / 2 + 1; k++) {
    hs[k] = I * k * gs[k];
  }
  // go to physical space
  my_fft_c2r(my_ffts, gs, es);
  my_fft_c2r(my_ffts, hs, fs);
  // product in physical space
  for (int i = 0; i < nitems; i++) {
    es[i] *= fs[i];
  }
  // go back to spectral space
  my_fft_r2c(my_ffts, es, hs);
  // normalise
  for (int k = 0; k < nitems / 2 + 1; k++) {
    hs[k] /= nitems;
  }
  for (int k = 0; k < nitems / 2 + 1; k++) {
    dgs[k] -= dt * hs[k];
  }
  free(hs);
  free(es);
  free(fs);
  return 0;
}

static int compute_adv_naive (
    const int nitems,
    const double dt,
    const fftw_complex * const gs,
    fftw_complex * const dgs
) {
  // compute convolution sum in the spectral space
  for (int l = - nitems / 2 + 1; l < nitems / 2 + 1; l++) {
    for (int m = - nitems / 2 + 1; m < nitems / 2 + 1; m++) {
      const int k = l + m;
      if (k < 0 || nitems / 2 < k) {
        continue;
      }
      fftw_complex gl = l < 0 ? conj(gs[-l]) : gs[l];
      fftw_complex gm = m < 0 ? conj(gs[-m]) : gs[m];
      dgs[k] -= dt * I * m * gl * gm;
    }
  }
  return 0;
}

int integrate (
    const int nitems,
    const double nu,
    const double dt,
    my_ffts_t * const my_ffts,
    const size_t advective_scheme_index,
    fftw_complex * const gs
) {
  // increment of function values
  fftw_complex * const dgs = calloc(nitems / 2 + 1, sizeof(fftw_complex));
  // advective term
  if (0 == advective_scheme_index) {
    compute_adv_fft_div(nitems, dt, my_ffts, gs, dgs);
  } else if (1 == advective_scheme_index) {
    compute_adv_fft_adv(nitems, dt, my_ffts, gs, dgs);
  } else {
    compute_adv_naive(nitems, dt, gs, dgs);
  }
  // diffusive term
  for (int k = 0; k < nitems / 2 + 1; k++) {
    dgs[k] -= nu * dt * k * k * gs[k];
  }
  // diffusive implicit treatment
  for (int k = 0; k < nitems / 2 + 1; k++) {
    dgs[k] /= 1. + 0.5 * nu * dt * k * k;
  }
  // update
  for (int k = 0; k < nitems / 2 + 1; k++) {
    gs[k] += dgs[k];
  }
  free(dgs);
  return 0;
}

