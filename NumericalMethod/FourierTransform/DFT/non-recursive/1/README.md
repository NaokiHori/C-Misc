# Fast Fourier Transform

Discrete Fourier transforms of a complex signal in `O(N log N)` operations, performing bit-reversal first.

## Caveat

This library is primarily developed for self-study purposes and is not intended for practical use.
The major limitations are as follows:

* Performance considerations are limited to the algorithmic context (not SIMD-ed, not cache-optimized).

* `nitems` (signal size) should be a power of `2`.

## Usage

See `main.c`.

