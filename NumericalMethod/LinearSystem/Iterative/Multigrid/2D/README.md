# 2D

Two-dimensional case.

## Setup

We consider an elliptic equation:

```math
\frac{1}{J}
\frac{\partial}{\partial \xi^i}
\left(
    \frac{J}{h_{{\xi}^i}}
    \frac{1}{h_{{\xi}^i}}
    \frac{\partial p}{\partial \xi^i}
\right)
=
q.
```

The second-order accurate central finite-difference approximation of this equation leads to a sparse linear system:

```math
\frac{1}{h_{i, j}}
\frac{1}{h_{i + \frac{1}{2}, j}}
p_{i + 1, j}
+
\left(
    -
    \frac{1}{h_{i, j}}
    \frac{1}{h_{i + \frac{1}{2}, j}}
    -
    \frac{1}{h_{i, j}}
    \frac{1}{h_{i - \frac{1}{2}, j}}
\right)
p_{i, j}
+
\frac{1}{h_{i, j}}
\frac{1}{h_{i - \frac{1}{2}, j}}
p_{i - 1, j}
+
\frac{1}{h_{i, j}}
\frac{1}{h_{i, j + \frac{1}{2}}}
p_{i, j + 1}
+
\left(
    -
    \frac{1}{h_{i, j}}
    \frac{1}{h_{i, j + \frac{1}{2}}}
    -
    \frac{1}{h_{i, j}}
    \frac{1}{h_{i, j - \frac{1}{2}}}
\right)
p_{i, j}
+
\frac{1}{h_{i, j}}
\frac{1}{h_{i, j - \frac{1}{2}}}
p_{i, j - 1}
=
q_{i, j},
```

where the scale factors are determined by the grid.

We impose the Neumann boundary condition at the edges:

```math
\frac{\partial p}{\partial \xi^i}
=
0.
```

