#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <complex.h>
#include <fftw3.h>
#include "my_ffts.h"
#include "integrate.h"
#include "output.h"

static const double pi = 3.141592653589793238;

static int init (
    const double length,
    const int nitems,
    my_ffts_t * const my_ffts,
    double * const xs,
    double * const fs,
    fftw_complex * const gs[N_ADVECTIVE_SCHEMES]
) {
  // initialize in physical space
  for (int i = 0; i < nitems; i++) {
    xs[i] = length * i / nitems;
    fs[i] = 1. + sin(xs[i]);
  }
  // transform to spectral space
  for (size_t n = 0; n < N_ADVECTIVE_SCHEMES; n++) {
    my_fft_r2c(my_ffts, fs, gs[n]);
  }
  // normalise FFT
  for (size_t n = 0; n < N_ADVECTIVE_SCHEMES; n++) {
    for (int i = 0; i < nitems / 2 + 1; i++) {
      gs[n][i] /= 1. * nitems;
    }
  }
  return 0;
}

int main (
    void
) {
  const double length = 2. * pi;
  const int nitems = 1024;
  const double nu = 2.e-2;
  const double dt = 0.5 * length / nitems;
  const double time_max = pi;
  my_ffts_t my_ffts = {0};
  initialize_my_ffts(nitems, &my_ffts);
  // x positions
  double * const xs = calloc(nitems, sizeof(double));
  // function values in physical space
  double * const fs = calloc(nitems, sizeof(double));
  // function values in spectral space
  // NOTE: try three different approaches
  fftw_complex * const gs[N_ADVECTIVE_SCHEMES] = {
    calloc(nitems / 2 + 1, sizeof(fftw_complex)),
    calloc(nitems / 2 + 1, sizeof(fftw_complex)),
    calloc(nitems / 2 + 1, sizeof(fftw_complex)),
  };
  // initialize function values in physical / spectral spaces
  init(length, nitems, &my_ffts, xs, fs, gs);
  // main loop
  double time = 0.;
  struct {
    double rate;
    double next;
    size_t counter;
  } schedule = {
    .rate = time_max / 10.,
    .next = schedule.rate,
    .counter = 1,
  };
  for (size_t n = 0; n < N_ADVECTIVE_SCHEMES; n++) {
    output(time, nitems, n, xs, fs);
  }
  while (time < time_max) {
    for (size_t n = 0; n < N_ADVECTIVE_SCHEMES; n++) {
      integrate(nitems, nu, dt, &my_ffts, n, gs[n]);
    }
    time += dt;
    if (schedule.next < time) {
      for (size_t n = 0; n < N_ADVECTIVE_SCHEMES; n++) {
        my_fft_c2r(&my_ffts, gs[n], fs);
        output(schedule.counter, nitems, n, xs, fs);
      }
      schedule.next += schedule.rate;
      schedule.counter += 1;
    }
  }
  free(xs);
  free(fs);
  for (size_t n = 0; n < N_ADVECTIVE_SCHEMES; n++) {
    free(gs[n]);
  }
  finalize_my_ffts(&my_ffts);
  return 0;
}

