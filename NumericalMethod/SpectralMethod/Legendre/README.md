# Legendre polynomials

## Prerequisite - Gaussian quadrature

The roots $x_i$ and weights $w_i$ are computed using the Golub-Welsch algorithm; namely, the eigenvalues of a tridiagonal matrix:

```math
A_{i, i + 1}
=
A_{i + 1, i}
=
\frac{
  i + 1
}{
  \sqrt{4 \left( i + 1 \right)^2 - 1}
},
\,\,
\text{zero otherwise},
\,\,
\left( i = 0, 1, \cdots N - 1 \right)
```

are the roots, and the first components of each eigenvector are the weights.

## Transform

A continuous function $a \left( x \right)$, $x \in \left[ -1, 1 \right]$ is expanded as

```math
a \left( x \right)
=
\sum_k
\alpha_k
P_k \left( x \right),
```

where $P_k \left( x \right)$ is the Legendre polynomials.
This is the inverse (backward) transform.

Integrating the equation in the domain with trial function $P_k$ yields:

```math
\int_{-1}^1
a \left( x \right)
P_l \left( x \right)
dx
=
\int_{-1}^1
\sum_k
\alpha_k
P_k \left( x \right)
P_l \left( x \right)
dx
=
\sum_k
\alpha_k
\frac{2}{2 k + 1}
\delta_{kl}
=
\alpha_l
\frac{2}{2 l + 1},
```

where we use the orthogonality:

```math
\int_{-1}^1
P_k \left( x \right)
P_l \left( x \right)
dx
=
\frac{2}{2 k + 1}
\delta_{kl}.
```

This gives the forward transform:

```math
\alpha_k
=
\frac{2 k + 1}{2}
\int_{-1}^1
a \left( x \right)
P_k \left( x \right)
dx.
```

The discrete backward transform is given by:

```math
a_i
=
\sum_{k = 0}^{N - 1}
P_{i, k}
\alpha_k,
\,\,
\left( i = 0, 1, \cdots, N - 1 \right)
```

where $a_i \equiv a \left( x_i \right)$ are the nodal value defined at Gauss roots $x_i$.
$P_{i, k}$ is a matrix projecting $N$ spectral values to $N$ nodal values:

```math
P_{i, k}
=
\begin{cases}
  1 & k = 0, \\
  x_i & k = 1, \\
  \frac{2 k - 1}{k}
  x_i
  P_{i, k - 1}
  -
  \frac{k - 1}{k}
  P_{i, k - 2} & \text{othrewise}.
\end{cases}
```

The forward transform is discretized as

```math
\alpha_k
=
\sum_{i = 0}^{N - 1}
\left(
  \frac{2 k + 1}{2}
  w_i
  P_{i, k}
\right)
a_i,
\,\,
\left( k = 0, 1, \cdots, N - 1 \right)
```

where $w_i$ is the $i$-th weight of Gaussian quadrature.

## Derivative

Differentiating the expansion by $x$ yields:

```math
\frac{
  da
}{
  dx
}
\left( x \right)
=
\sum_k
\alpha_k
\frac{
  dP_k
}{
  dx
}
\left( x \right),
```

or discretely at the collocation nodes $x_i$:

```math
a_i^{\prime}
=
\sum_k
P_{i, k}^{\prime}
\alpha_k.
```

By utilizing the following recurrence relation:

```math
P_k^{\left( n \right)}
=
\left( k + n - 1 \right)
P_{k - 1}^{\left( n - 1 \right)}
+
x
P_{k - 1}^{\left( n \right)},
```

the linear operator $P_{i, k}^{\prime}$ can be computed as:

```math
P_{i, k}^{\prime}
=
\begin{cases}
  0 & k = 0, \\
  k P_{i, k - 1}
  +
  x_i P_{i, k - 1}^{\prime} & \text{otherwise}.
\end{cases}
```

Higher-order derivatives can be computed in the same manner.

## Reference

- [Legendre polynomials - Wikipedia](https://en.wikipedia.org/w/index.php?title=Legendre_polynomials&oldid=1370673668)

