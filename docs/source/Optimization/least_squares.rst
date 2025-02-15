#############
Least squares
#############

We consider a set of :math:`N` points :math:`(x_i, y_i)` (where :math:`i = 0, 1, \dots, N-1`) on a two-dimensional plane.
We aim at fitting them using a polynomial of degree :math:`M-1`:

.. math::

    y
    =
    f(x)
    =
    \sum_{j = 0}^{M - 1} w_j x^j.

For later convenience, the partial derivative with respect to :math:`w_k` is given by:

.. math::

    g(x) \equiv \frac{\partial f}{\partial w_k}
    =
    x^k

for :math:`k = 0, 1, \dots, M - 1`.

The error function is defined as:

.. math::

    J(w_j)
    \equiv
    \sum_{i = 0}^{N - 1} \left( \sum_{j = 0}^{M - 1} w_j x_i^j - y_i \right)^2

and the goal is to minimize this error.

Since there are no constraints, setting the stationary condition gives:

.. math::

    \frac{\partial J}{\partial w_k}
    =
    2 \sum_{i = 0}^{N - 1} \left[ f(x_i) - y_i \right] g(x_i)
    =
    2 \sum_{i = 0}^{N - 1} \left( \sum_{j = 0}^{M - 1} w_j x_i^j - y_i \right) x_i^k
    =
    0,

which leads to the normal equations:

.. math::

    \sum_{j = 0}^{M - 1} \left( \sum_{i = 0}^{N - 1} x_i^{j + k} \right) w_j
    =
    \sum_{i = 0}^{N - 1} x_i^k y_i.

Explicitly, this system can be written in matrix form as:

.. math::

    \begin{pmatrix}
        \sum_i x_i^0 & \sum_i x_i^1 & \cdots & \sum_i x_i^{M - 1} \\
        \sum_i x_i^1 & \sum_i x_i^2 & \cdots & \sum_i x_i^{M} \\
        \vdots & \vdots & \ddots & \vdots \\
        \sum_i x_i^{M - 1} & \sum_i x_i^M & \cdots & \sum_i x_i^{2M - 2}
    \end{pmatrix}
    \begin{pmatrix}
        w_0 \\
        w_1 \\
        \vdots \\
        w_{M - 1}
    \end{pmatrix}
    =
    \begin{pmatrix}
        \sum_i x_i^0 y_i \\
        \sum_i x_i^1 y_i \\
        \vdots \\
        \sum_i x_i^{M - 1} y_i \\
    \end{pmatrix}.

Solving for :math:`w_k` (:math:`k = 0, 1, \dots, M - 1`) provides the best-fit polynomial coefficients.

