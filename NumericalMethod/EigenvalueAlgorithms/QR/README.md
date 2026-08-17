# QR algorithm

QR algorithm, step-by-step.

## Givens rotation

Baseline of the following steps: QR decomposition by means of Givens rotation.

## Naive

Naive implementation of QR algorithm.

- Givens rotation

## Tridiagonal

For symmetric and tridiagonal input.

- Givens rotation
- symmetric tridiagonal input

## Wilkinson shift

Faster convergence of each eigenvalue.

- Givens rotation
- symmetric tridiagonal input
- Wilkinson's shift

## Deflation

Avoid modification on already-converged eigenvalues, improving convergence.

- Givens rotation
- symmetric tridiagonal input
- Wilkinson's shift
- bottom sequential deflation (sub-optimal)

## Implicit

Improved time / space complexity by facilitating the sparse nature of the input / intermediate matrices.

- Givens rotation
- symmetric tridiagonal input
- Wilkinson's shift
- bottom sequential deflation (sub-optimal)
- bulge chasing

