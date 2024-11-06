# Fast Fourier Transform

Discrete Fourier transforms of a complex signal in `O(N log N)` operations.

## Caveat

This library is primarily developed for self-study purposes and is not intended for practical use.
The major limitations are as follows:

* Performance considerations are limited to the algorithmic context (not SIMD-ed, not cache-optimized).

* `nitems` (signal size) should be a power of `2` or `3` (`2 ^ N`, `3 ^ N`), or their products; otherwise this library uses a naive `O(N ^ 2)` algorithm. For example, a signal whose length is `468` is computed using an efficient divide-and-conquer technique for the `2 ^ 2` and `3 ^ 2` sub parts while a sub problem of `N = 13` is done naively.

## Usage

See `main.c`.

## Reference

- [Cooley–Tukey FFT algorithm](https://en.wikipedia.org/wiki/Cooley–Tukey_FFT_algorithm)

