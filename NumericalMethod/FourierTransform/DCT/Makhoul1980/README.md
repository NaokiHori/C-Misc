# Fast Cosine Transform

*I suggest to check the other directory Lee1984, which is simpler.*

Discrete cosine transforms (type 2 and 3) in O(N log N) operations, based on Makhoul 1980, without `complex.h`.

## Caveat

This library is primarily developed for self-study purposes and is not intended for practical use.
The major limitations are as follows:

* Performance considerations are limited to the algorithmic context (not SIMD-ed, not cache-optimized).

* `nitems` (signal size) should be `2^N` (for now, this may be extended in the near future).

* In-place transforms are not implemented (`isignal` and `osignal` should be `restrict` to each other).

## Usage

The interface is designed to be similar to [FFTW3](https://www.fftw.org).
See the following pseudo code:

```c
// Include a header
#include "sdct.h"

// Prepare signals
double * isignal = malloc(nitems * sizeof(double));
double * osignal = malloc(nitems * sizeof(double));
// Initialize input somehow
init_input(nitems, isignal);

// Make a plan
sdct_plan_t * plan = NULL;
if (0 != sdct_init_plan(nitems, true, &plan)) {
  // Failed to create a plan, possibly one of the following:
  //   - Memory allocation failed
  //   - nitems is not a power of 2
  //   - nitems is smaller than 4
  return 1;
}

// Perform DCT and iDCT, as many times as you want
// Forward (DCT type 2)
sdct_exec_f(plan, isignal, osignal);
// Backward (DCT type 3)
sdct_exec_b(plan, isignal, osignal);

// Clean up a plan
sdct_destroy_plan(plan);
// Deallocate signals
free(isignal);
free(osignal);
```

See `main.c`.

## References

- Cooley and Tukey, "An algorithm for the machine calculation of complex Fourier series", *Math. Comput.*, 1965

- Makhoul, "A Fast Cosine Transform in One and Two Dimensions", *IEEE T. Acoust. Speech*, 1980

- [Wikipedia - Cooley–Tukey FFT algorithm](https://en.wikipedia.org/wiki/Cooley–Tukey_FFT_algorithm)

