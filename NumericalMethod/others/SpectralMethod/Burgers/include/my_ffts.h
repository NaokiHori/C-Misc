#if !defined(MY_FFTS_T)
#define MY_FFTS_T

#include <fftw3.h>

typedef struct {
  int nitems;
  double * rbuf;
  fftw_complex * cbuf;
  fftw_plan r2c;
  fftw_plan c2r;
} my_ffts_t;

extern int initialize_my_ffts (
    const int nitems,
    my_ffts_t * const my_ffts
);

extern int finalize_my_ffts (
    my_ffts_t * const my_ffts
);

extern int my_fft_r2c (
    my_ffts_t * const my_ffts,
    const double * const rbuf,
    fftw_complex * const cbuf
);

extern int my_fft_c2r (
    my_ffts_t * const my_ffts,
    const fftw_complex * const cbuf,
    double * const rbuf
);

#endif // MY_FFTS_T
