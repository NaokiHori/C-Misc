# Steepest Descent Method

Steepest descent method to solve symmetric and positive-definite linear systems.

## Method

### Setup

We aim to solve a linear system

```math
A_{ij} x_j = b_i,
```

where `A` is a symmetric and positive-definite matrix.
Instead of directly treating this linear system, we consider the quadratic form:

```math
f
\equiv
\frac{1}{2} A_{ij} x_i x_j
-
b_i
x_i,
```

whose gradient leads to

```math
\frac{\partial f}{\partial x_k}
=
\frac{1}{2} A_{ij} \frac{\partial x_i}{\partial x_k} x_j
+
\frac{1}{2} A_{ij} x_i \frac{\partial x_j}{\partial x_k}
-
b_i
\frac{\partial x_i}{\partial x_k}.
```

By utilizing an identity

```math
\frac{\partial x_i}{\partial x_j}
=
\delta_{ij},
```

we obtain

```math
\frac{\partial f}{\partial x_k}
=
\frac{1}{2} A_{ij} \delta_{ik} x_j
+
\frac{1}{2} A_{ij} x_i \delta_{jk}
-
b_i
\delta_{ik}
=
\frac{1}{2} A_{kj} x_j
+
\frac{1}{2} A_{ik} x_i
-
b_k.
```

Since the indices are dummy and `A` is symmetric, this relation can be simplified as

```math
\frac{\partial f}{\partial x_i}
=
A_{ij} x_j
-
b_i.
```

As a result, the original problem can be considered differently: finding the extremum of `f`.

### Numerical treatment

We aim to find `x_i` at which the gradient of `f` is zero.
To this end, we rely on an iterative process.

Given `x_i^n` (with the superscript being the step), we would like find `x_i^{n + 1}` which is "better" in terms of the error.
The error is quantitatively defined as the e.g. L2 norm of the residual vector:

```math
r_i^n
\equiv
A_{ij} x_j^n
-
b_i.
```

To update `x_i`, we need to find the increment vector

```math
\Delta x_i
\equiv
x_i^{n + 1}
-
x_i^n,
```

which is chosen to be parallel to the residual vector:

```math
\Delta x_i
=
\eta
r_i^n.
```

Here the scalar is determined to satisfy

```math
r_i^{n}
r_i^{n + 1}
=
0.
```

Since the residual vector at the next step can be described using the current information:

```math
r_i^{n + 1}
=
A_{ij} x_j^{n + 1}
-
b_i
=
A_{ij} \left( x_j^n + \eta r_j^n \right)
-
b_i
=
r_i^n
+
\eta A_{ij} r_j^n,
```

the scalar quantity is given by

```math
\eta
\equiv
-
\frac{
    r_i^n r_i^n
}{
    A_{ij} r_i^n r_j^n
}.
```
## Reference

- [Gradient descent - Wikipedia](https://en.wikipedia.org/wiki/Gradient_descent#Solution_of_a_linear_system)

