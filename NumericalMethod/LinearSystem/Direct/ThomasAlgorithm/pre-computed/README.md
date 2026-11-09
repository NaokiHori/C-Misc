# Thomas algorithm

## Prerequisite

The algorithm is composed of two steps: forward sweep and backward substitution.

### Forward sweep

The forward sweep updates the upper-diagonal components $u_i$:

```math
u_0^{\prime}
=
\frac{
  u_0
}{
  m_0
},
```

```math
u_i^{\prime}
=
\frac{
  u_i
}{
  m_i - l_i u_{i - 1}^{\prime}
},
```

along with the right-hand-side vector $q_0$:

```math
q_0^{\prime}
=
\frac{
  q_0
}{
  m_0
},
```

```math
q_i^{\prime}
=
\frac{
  q_i - l_i q_{i - 1}^{\prime}
}{
  m_i - l_i u_{i - 1}^{\prime}
},
```

where $i = 1, 2, \cdots, N - 1$.

Note that $q_i$ can be updated in-place, whereas $u_i$ uses another buffer to be reused.

### Backward substitution

The backward substitution updates the right-hand-side vector following

```math
q_i^{\prime\prime}
=
q_i^{\prime}
-
u_i^{\prime}
q_{i + 1}^{\prime\prime},
```

where $i = N - 2, N - 3, \cdots 0$.

### Reformulation

For later convenience, we define

```math
k_i
\equiv
\begin{cases}
  \frac{
    1
  }{
    m_i
  }
  &
  i = 0, \\
  \frac{
    1
  }{
    m_i
    -
    l_i
    u_{i - 1}
    k_{i - 1}
  }
  &
  i = 1, 2, \cdots, N - 1,
\end{cases}
```

to write the forward sweep:

```math
q_0^{\prime}
=
k_0
q_0,
```

```math
q_i^{\prime}
=
k_i
\left(
  q_i
  -
  l_i q_{i - 1}^{\prime}
\right),
\,\,
\left( i = 1, 2, \cdots, N - 1 \right)
```

and the backward substitution:

```math
q_i^{\prime\prime}
=
q_i^{\prime}
-
k_i
u_i
q_{i + 1}^{\prime\prime}.
\,\,
\left( i = N - 2, N - 3, \cdots, 0 \right)
```

## For quasi-diagonally dominant cases

We consider cases $- C - l_i - u_i = m_i$, which happen, for example, when we deal with multi-dimensional discrete Poisson equations:

```math
l_i
f_{i - 1}
+
m_i
f_i
+
u_i
f_{i + 1}
=
g_i,
```

where $0 < l_i$, $0 < u_i$, $0 \le C$ and $m_i = - l_i - u_i - C < 0$.

The denominator of $k_i$ calculation leads to

```math
d_i
\equiv
\begin{cases}
  m_i
  &
  i = 0, \\
  m_i
  -
  \frac{
    l_i
    u_{i - 1}
  }{
    d_{i - 1}
  }
  &
  \text{otherwise}.
\end{cases}
```

For visibility, we define

```math
D_i
\equiv
-
d_i,
```

giving

```math
D_i
=
\begin{cases}
  -
  m_i
  =
  l_i
  +
  u_i
  +
  C
  >
  0
  &
  i = 0, \\
  -
  m_i
  -
  \frac{
    l_i
    u_{i - 1}
  }{
    D_{i - 1}
  }
  =
  \left(
    l_i
    +
    u_i
    +
    C
  \right)
  -
  \frac{
    l_i
    u_{i - 1}
  }{
    D_{i - 1}
  }
  &
  \text{otherwise}.
\end{cases}
```

Here, we notice that catastrophic cancellation may occur when the subtraction is computed as it is.

As a remedy, we introduce

```math
r_i
\equiv
D_i
-
u_i
```

to obtain

```math
r_i
=
\begin{cases}
  C
  +
  l_i
  &
  i = 0, \\
  C
  +
  l_i
  -
  \frac{
    l_i
    u_{i - 1}
  }{
    u_{i - 1} 
    + 
    r_{i - 1}
  }
  =
  C
  +
  l_i
  \frac{
    r_{i - 1}
  }{
    u_{i - 1}
    +
    r_{i - 1}
  }
  &
  \text{otherwise}.
\end{cases}
```

Here, we see that every operand in the recurrence is non-negative, so only additions, multiplications and divisions occur.

Finally, $k_i$, which is the vector used inside the solver, is computed as:

```math
k_i
=
-
\frac{
  1
}{
  u_i
  +
  r_i
}.
```

## Reference

- Grassmann et al., Oper. Res. (33:5), 1985

