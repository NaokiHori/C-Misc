# Fast Cosine Transform

Discrete cosine transforms (type 2 and 3) in O(N log N) operations, based on Lee 1984.

## Caveat

This library is primarily developed for self-study purposes and is not intended for practical use.
The major limitations are as follows:

* Performance considerations are limited to the algorithmic context (not SIMD-ed, not cache-optimized).

* `nitems` (signal size) should be `2 ^ N` (or similar).

## Usage

See `main.c`.

## References

- Lee, "A New Algorithm to Compute the Discrete Cosine Transform", *IEEE T. Acoust. Speech*, 1984

## Algorithm

A documentation regarding the method and its derivation is available in `docs`.
Install [Sphinx](https://www.sphinx-doc.org/en/master/), `make all`, open `build/index.html`.

