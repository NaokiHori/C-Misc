##################
First-order method
##################

A first-order explicit Runge-Kutta method is nothing else but the Euler-forward scheme:

.. math::

    & q^0 = p^n

    & q^1 = p^n + a_{1, 0} f \left( q^0 \right) \Delta t

    & p^{n + 1} = q^1

The Butcher tableau is

.. math::

    \begin{array}{c|c}
    b_0 & a_{0,0} \\
    \hline
    & a_{1,0} \\
    \end{array}
    =
    \begin{array}{c|c}
    0 & 0 \\
    \hline
    & 1 \\
    \end{array}

This is already a low-storage scheme:

.. math::

    \begin{array}{c|c}
    k & 0 \\
    \hline
    \beta^k  & 0 \\
    \gamma^k & 1
    \end{array}

