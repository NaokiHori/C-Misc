#include <stdlib.h>
#include <string.h>
#include "my_ffts.h"

int initialize_my_ffts (
    const int nitems,
    my_ffts_t * const my_ffts
) {
  my_ffts->nitems = nitems;
  double ** const rbuf = &my_ffts->rbuf;
  fftw_complex ** const cbuf = &my_ffts->cbuf;
  fftw_plan * const r2c = &my_ffts->r2c;
  fftw_plan * const c2r = &my_ffts->c2r;
  *rbuf = fftw_malloc(nitems * sizeof(double));
  *cbuf = fftw_malloc((nitems / 2 + 1) * sizeof(fftw_complex));
  *r2c = fftw_plan_dft_r2c_1d(nitems, *rbuf, *cbuf, FFTW_MEASURE);
  *c2r = fftw_plan_dft_c2r_1d(nitems, *cbuf, *rbuf, FFTW_MEASURE);
  return 0;
}

int finalize_my_ffts (
    my_ffts_t * const my_ffts
) {
  free(my_ffts->rbuf);
  free(my_ffts->cbuf);
  fftw_destroy_plan(my_ffts->r2c);
  fftw_destroy_plan(my_ffts->c2r);
  return 0;
}

int my_fft_r2c (
    my_ffts_t * const my_ffts,
    const double *rbuf,
    fftw_complex *cbuf
) {
  const int nitems = my_ffts->nitems;
  memcpy(my_ffts->rbuf, rbuf, sizeof(double) * nitems);
  fftw_execute(my_ffts->r2c);
  memcpy(cbuf, my_ffts->cbuf, sizeof(fftw_complex) * (nitems / 2 + 1));
  return 0;
}

int my_fft_c2r (
    my_ffts_t * const my_ffts,
    const fftw_complex *cbuf,
    double *rbuf
) {
  const int nitems = my_ffts->nitems;
  memcpy(my_ffts->cbuf, cbuf, sizeof(fftw_complex) * (nitems / 2 + 1));
  fftw_execute(my_ffts->c2r);
  memcpy(rbuf, my_ffts->rbuf, sizeof(double) * nitems);
  return 0;
}

