# Real-Valued Fast Fourier Transform

Discrete Fourier transforms of real-valued signals and the corresponding inverse transform in `O(N log N)` operations.
Due to the complex-conjugate nature of the resulting signal, the output signal `X_k` is ordered as follows.

```
        0 : Re(X_{        0})
        1 : Re(X_{        1})
...
N / 2 - 1 : Re(X_{N / 2 - 1})
N / 2     : Re(X_{N / 2    })
N / 2 + 1 : Im(X_{N / 2 + 1})
N / 2 + 2 : Im(X_{N / 2 + 2})
...
N     - 2 : Im(X_{N     - 2})
N     - 1 : Im(X_{N     - 1})
```

## Caveat

This library is primarily developed for self-study purposes and is not intended for practical use.
The major limitations are as follows:

* Performance considerations are limited to the algorithmic context (not SIMD-ed, not cache-optimized).

* `nitems` (signal size) should be a power of `2` (`2 ^ N`).

## Usage

See `main.c`.

## Reference

[FFTが適用できる変換 1.2 (in Japanese)](https://www.kurims.kyoto-u.ac.jp/~ooura/fftman/ftmn2_12.html#sec2_1_2)

