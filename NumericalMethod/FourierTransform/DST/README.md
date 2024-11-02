# Fast Sine Transform

Discrete sine transforms (type 2 and 3) in O(N log N) operations, based on Lee 1984.

## Caveat

This library is primarily developed for self-study purposes and is not intended for practical use.
The major limitations are as follows:

* Performance considerations are limited to the algorithmic context (not SIMD-ed, not cache-optimized).

* `nitems` (signal size) should be `2 ^ N` (or similar).

## Usage

See `main.c`.

## Reference

- Lee, "A New Algorithm to Compute the Discrete Cosine Transform", *IEEE T. Acoust. Speech*, 1984

