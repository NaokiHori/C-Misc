# Non-Uniform

The grid is stretched.

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
q,
```

or in 1D:

```math
\frac{1}{h}
\frac{\partial}{\partial \xi}
\left(
    \frac{1}{h}
    \frac{\partial p}{\partial \xi}
\right)
=
q.
```

The second-order finite-difference approximation of this equation leads to a tri-diagonal linear system:

```math
\frac{1}{h_i}
\frac{1}{h_{i + \frac{1}{2}}}
p_{i + 1}
+
\left(
    -
    \frac{1}{h_i}
    \frac{1}{h_{i + \frac{1}{2}}}
    -
    \frac{1}{h_i}
    \frac{1}{h_{i - \frac{1}{2}}}
\right)
p_i
+
\frac{1}{h_i}
\frac{1}{h_{i - \frac{1}{2}}}
p_{i - 1}
=
q_i,
```

where the scale factors are determined by the grid.

We impose the Neumann boundary condition at the edges:

```math
\frac{\partial p}{\partial \xi^i}
=
0.
```
