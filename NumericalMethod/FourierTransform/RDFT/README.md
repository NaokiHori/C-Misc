# Real-Valued Fast Fourier Transform

Discrete Fourier transforms of real-valued signals and the corresponding inverse transform in `O(N log N)` operations.
The input / output signals are pure real, which is achieved by ordering the spectral data `X_k`as `Re(X_0)`, `Re(X_1)`, ..., `Re(X_{N / 2 - 1})`, `Re(X_{N / 2})`, `Im(X_{N / 2 - 1})`, `Im(X_{N / 2 - 2})`, ..., `Im(X_2)`, `Im(X_1)`.

## Caveat

This library is primarily developed for self-study purposes and is not intended for practical use.
The major limitations are as follows:

* Performance considerations are limited to the algorithmic context (not SIMD-ed, not cache-optimized).

* `nitems` (signal size) should be a power of `2` (`2 ^ N`); otherwise this library uses a naive `O(N ^ 2)` algorithm.

## Usage

See `main.c`.

## Algorithm

A documentation regarding the method and its derivation is available in `docs`.
Install [Sphinx](https://www.sphinx-doc.org/en/master/), `make all`, open `build/index.html`.

