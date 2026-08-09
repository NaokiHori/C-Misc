# Conjugate Gradient Method

Conjugate gradient method to solve symmetric and positive-definite linear systems.

## Method

### Setup

We aim to find the extremum of $f$:

```math
f
\equiv
\frac{1}{2} A_{ij} x_i x_j
-
b_i
x_i.
```

See [steepest descent method](../SteepestDescent/README.md).

### Numerical treatment

We use an iterative technique to find the solution.
Namely, given $x_i^n$ (with the superscript being the step), we would like find $x_i^{n + 1}$ which is "better" in terms of the error.
The error is quantitatively defined as the e.g. L2 norm of the residual vector:

```math
r_i^n
\equiv
A_{ij} x_j^n
-
b_i.
```

To update $x_i$, we need to find the increment vector:

```math
\Delta x_i
\equiv
x_i^{n + 1}
-
x_i^n.
```

The conjugate-gradient method defines it to be

```math
\Delta x_i
=
\eta
p_i^n,
```

where $p_i^n$ is the conjugate search direction vector computed as:

```math
p_i^n
=
r_i^n
+
\theta
p_i^{n - 1},
```

with $p_i^0 = r_i^0$ (or we could say $\theta = 0$ at the first step).

Since the two vectors $p_i^n$ and $p_i^{n - 1}$ are conjugate by design:

```math
A_{ij}
p_i^n
p_j^{n - 1}
=
0,
```

we obtain

```math
A_{ij}
p_i^n
p_j^{n - 1}
=
A_{ij}
r_i^n
p_j^{n - 1}
+
\theta
A_{ij}
p_i^{n - 1}
p_j^{n - 1}
=
0,
```

or equivalently

```math
\theta
=
-
\frac{
    A_{ij}
    r_i^n
    p_j^{n - 1}
}{
    A_{ij}
    p_i^{n - 1}
    p_j^{n - 1}
}.
```

To proceed, we focus on the relation:

```math
r_i^{n + 1}
=
A_{ij} x_j^{n + 1}
-
b_i
=
A_{ij} \left( x_j^n + \eta p_j^n \right)
-
b_i
=
r_i^n
+
\eta A_{ij} p_j^n,
\quad
\cdots
\quad
\left[ 1 \right]
```

which is used to reformulate $\theta$:

```math
\theta
=
-
\frac{
    r_i^n
    \left(
        \eta
        A_{ij}
        p_j^{n - 1}
    \right)
}{
    p_i^{n - 1}
    \left(
        \eta
        A_{ij}
        p_j^{n - 1}
    \right)
}
=
-
\frac{
    r_i^n
    r_i^n
    -
    r_i^{n - 1}
    r_i^n
}{
    r_i^n
    p_i^{n - 1}
    -
    r_i^{n - 1}
    p_i^{n - 1}
}.
```

Due to the orthogonalities, we obtain

```math
\theta
=
\frac{
    r_i^n r_i^n
}{
    r_i^{n - 1} p_i^{n - 1}
}.
```

Also, by substituting $\left[ 1 \right]$ into the orthogonality condition (requested by design):

```math
p_i^n
r_i^{n + 1}
=
0,
```

we obtain

```math
\eta
\equiv
-
\frac{
    p_i^n r_i^n
}{
    A_{ij} p_i^n p_j^n
}.
```

Finally, by focusing on the relation:

```math
p_i^n r_i^n
=
\left( r_i^n + \theta p_i^{n - 1} \right) r_i^n
=
r_i^n r_i^n,
\quad
\left(
    \because
    {
        p_i^{n - 1} r_i^n = 0
    }
\right)
```

we obtain efficient ways to evaluate the two scalar quantities:

```math
\eta
=
-
\frac{
    r_i^n r_i^n
}{
    A_{ij} p_i^n p_j^n
},
```

```math
\theta
=
\frac{
    r_i^n r_i^n
}{
    r_i^{n - 1} r_i^{n - 1}
}.
```

## Reference

- [Conjugate gradient method - Wikipedia](https://en.wikipedia.org/wiki/Conjugate_gradient_method)

