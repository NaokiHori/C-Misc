# Sequential Minimal Optimization

## Problem setup

We aim at maximizing

```math
L \left( \vec{a} \right)
=
\sum_i a_i
-
\frac{1}{2}
\sum_i
\sum_j
a_i
a_j
t_i
t_j
k_{ij}
```

subject to

```math
0 \le a_i \le C\,\, \forall i
```

and 

```math
\sum_i
a_i
t_i
=
0.
```

Here, $k_{ij}$ is a short-hand form of kernel $k \left( \vec{x}_i, \vec{x}_j \right)$ (e.g., radial basis function kernel).

By introducing Lagrange multipliers $0 \le \mu_i$ and $\nu$ (unrestricted in sign), we define a Lagrangian:

```math
\Lambda
\equiv
L
+
\sum_i
\mu_i
a_i
-
\nu
\sum_i
a_i
t_i,
```

whose stationary condition (which should be satisfied at the optimal point) gives

```math
1
-
t_i
\sum_j
a_j
t_j
k_{ij}
+
\mu_i
-
\nu t_i
=
0 \,\, \forall i.
```

With the complementary slackness $\mu_i a_i = 0$, this indicates that all $i$ should satisfy

```math
a_i
=
0
\land
0
\le
t_i \left( \sum_j a_j t_j k_{ij} + \nu \right)
-
1,
```

or

```math
a_i
=
C
\land
t_i \left( \sum_j a_j t_j k_{ij} + \nu \right)
-
1
\le
0,
```

or

```math
0
<
a_i
\land
t_i \left( \sum_j a_j t_j k_{ij} + \nu \right)
-
1
=
0
```

at the optimal point.

## Method

We aim to find the maximum of the objective function $L$ in a sequential manner.
Although $L$ is convex, due to the equality constraint, we cannot update each component of $\vec{a}$ independently.
With the sequential minimal optimization, we update two components $a_i$ and $a_j$ simultaneously so that the correction does not break the equality condition:

```math
t_i a_i
+
t_j a_j
=
\xi
\equiv
-
\sum_{n \neq i, j}
t_n a_n.
```

To get a concrete formulation, we split $L$ into the $i,j$-related part and others:

```math
\sum_n
a_n
=
a_i
+
a_j
+
\sum_{n \neq i, j} a_n
```

and compute the derivative with respect to $a_i$:

```math
\frac{
    \partial L
}{
    \partial a_i
}
=
\left(
    - k_{ii}
    + 2 k_{ij}
    - k_{jj}
\right)
a_i
-
t_i t_j
+
1
-
\xi
t_i
\left(
    k_{ij}
    -
    k_{jj}
\right)
-
t_i
\left(
    \sum_{n \neq i, j}
    a_n
    t_n
    k_{in}
    -
    \sum_{n \neq i, j}
    a_n
    t_n
    k_{jn}
\right).
```

Requesting this to be zero leads to the way to obtain new $a_i$:

```math
a_i
=
\frac{
    t_i t_j
    -
    1
    +
    \xi
    t_i
    \left(
        k_{ij}
        -
        k_{jj}
    \right)
    +
    t_i
    \left(
        \sum_{n \neq i, j}
        a_n
        t_n
        k_{in}
        -
        \sum_{n \neq i, j}
        a_n
        t_n
        k_{jn}
    \right)
}{
    - k_{ii}
    + 2 k_{ij}
    - k_{jj}
},
```

followed by 

```math
a_j
=
t_j
\left(
    \xi
    -
    a_i
    t_i
\right).
```

Although this keeps the equality constraint satisfied, doing this naively may break the inequality constraints $0 \le a_i \le C, 0 \le a_j \le C$.
To fulfill these conditions as well, we introduce a cap $l \le a_i \le u$:

```math
l
\equiv
\begin{cases}
    \max \left( 0, t_i \xi - C \right) & \text{if} \,\, t_i = t_j, \\
    \max \left( 0, t_i \xi \right) & \text{otherwise},
\end{cases}
```

```math
u
\equiv
\begin{cases}
    \min \left( C, t_i \xi \right) & \text{if} \,\, t_i = t_j, \\
    \min \left( C, C + t_i \xi \right) & \text{otherwise},
\end{cases}
```

before updating $a_j$.

We need to choose two datasets $i$ and $j$, which we adopt the following approach proposed by Keerthi et al.:

```math
i 
=
\arg \max_m \left\{ t_m g_m | m \in I_{upper} \right\},
```

```math
j
=
\arg \min_m \left\{ t_m g_m | m \in I_{lower} \right\},
```

where

```math
I_{upper}
\equiv
\left\{ m | \left( t_m = 1 \land a_m < C \right) \lor \left( t_m = -1 \land 0 < a_m \right) \right\},
```

```math
I_{lower}
\equiv
\left\{ m | \left( t_m = 1 \land 0 < a_m \right) \lor \left( t_m = -1 \land a_m < C \right) \right\},
```

with

```math
g_m
\equiv
\frac{\partial L}{\partial a_m}
=
1
-
t_m
\sum_n
a_n t_n k_{mn}.
```

In words, $i$ / $j$ is the dataset with the largest / smallest $t_m g_m$ among those $t_m a_m$ can increase / decrease, respectively.

The quantity $t_m g_m$ can also be used to determine the convergence: we stop iterating when the following condition is satisfied:

```math
t_i g_i
-
t_j g_j
<
\epsilon,
```

where $\epsilon$ is the tolerance which quantify the KKT violation.
Also, we compute $\nu$ as

```math
\nu
=
\frac{1}{2} t_i g_i
+
\frac{1}{2} t_j g_j.
```

## Reference

- C. M. Bishop, Springer, 2006
- [Sequential minimal optimization - Wikipedia](https://en.wikipedia.org/w/index.php?title=Sequential_minimal_optimization&oldid=1373933737)
- [Keerthi et al., *Neural Comput.* (**13**), 2001](https://doi.org/10.1162/089976601300014493)

