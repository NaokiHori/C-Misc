# Burgers

Solving Burgers equation by means of the `Fourier-Galerkin` method.

## Dependency

- [`fftw3`](https://www.fftw.org)

## Methods

### Treatment of advective term

The advective term `-duu/dx` is computed in three different ways:

- Computing in the physical domain as `-duu/dx`: divergence form
- Computing in the physical domain as `-udu/dx`: advective form
- Computing in the spectral domain: evaluating the convolution sum directly.

They are computed in parallel to show their agreements.

### Aliasing error

No treatment is considered to eliminate the aliasing error for brevity.

