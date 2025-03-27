####################
Second-order methods
####################

Second-order explicit Runge-Kutta methods are given by

.. math::

    & q^0 = p^n

    & q^1 = p^n + a_{1,0} f \left( q^0 \right) \Delta t

    & q^2 = p^n + a_{2,0} f \left( q^0 \right) \Delta t + a_{2,1} f \left( q^1 \right) \Delta t

    & p^{n + 1} = q^2

The Butcher tableau is

.. math::

    \begin{array}{c|cc}
    b_0 & a_{0,0} & a_{0,1} \\
    b_1 & a_{1,0} & a_{1,1} \\
    \hline
    & a_{2,0} & a_{2,1} \\
    \end{array}
    =
    \begin{array}{c|cc}
    0 & 0 & 0 \\
    \alpha & \alpha & 0 \\
    \hline
    & 1 - \frac{1}{2 \alpha} & \frac{1}{2 \alpha} \\
    \end{array}

where the elements are constrained by a free parameter :math:`\alpha`.

The low-storage schemes can be derived as follows.

First, we substitute :math:`p^n` with :math:`q^0` and eliminate :math:`q^1` from the third relation:

.. math::

    & q^0 = p^n

    & q^1 = q^0 + a_{1,0} f \left( q^0 \right) \Delta t

    & q^2 = q^1 + \left( - a_{1,0} + a_{2,0} \right) f \left( q^0 \right) \Delta t + a_{2,1} f \left( q^1 \right) \Delta t

    & p^{n + 1} = q^2

Then the pre-factors are rearranged:

.. math::

    & q^0 = p^n

    & q^1 = q^0 + a_{1,0} f \left( q^0 \right) \Delta t

    & q^2 = q^1 + a_{2,1} \left[ \frac{- a_{1,0} + a_{2,0}}{a_{2,1}} f \left( q^0 \right) + f \left( q^1 \right) \right] \Delta t

    & p^{n + 1} = q^2

Finally, we define :math:`r^k`, which store the computed right-hand-side terms of the differential equation:

.. math::

    & q^0 = p^n

    & r^0 = f \left( q^0 \right)

    & q^1 = q^0 + a_{1,0} r^0 \Delta t

    & r^1 = \frac{- a_{1,0} + a_{2,0}}{a_{2,1}} f \left( q^0 \right) + f \left( q^1 \right)

    & q^2 = q^1 + a_{2,1} r^1 \Delta t

    & p^{n + 1} = q^2

to obtain

.. math::

    \begin{array}{c|cc}
    k & 0 & 1 \\
    \hline
    \beta^k  & 0       & \frac{- a_{1,0} + a_{2,0}}{a_{2,1}} \\
    \gamma^k & a_{1,0} & a_{2,1}
    \end{array}
    =
    \begin{array}{c|cc}
    k & 0 & 1 \\
    \hline
    \beta^k  & 0      & - 2 \alpha^2 + 2 \alpha - 1 \\
    \gamma^k & \alpha & \frac{1}{2 \alpha}
    \end{array}

Unlike the first-order method, there are countless low-storage schemes yielding second-order accuracy due to the presence of the free parameter :math:`\alpha`.
The following methods are popular among others.

*************************
Explicit mid-point method
*************************

.. math::

    \alpha = \frac{1}{2}
    \Rightarrow
    \begin{array}{c|cc}
    k & 0 & 1 \\
    \hline
    \beta^k  & 0 & - \frac{1}{2} \\
    \gamma^k & \frac{1}{2} & 1
    \end{array}

*************
Heun's method
*************

.. math::

    \alpha = 1
    \Rightarrow
    \begin{array}{c|cc}
    k & 0 & 1 \\
    \hline
    \beta^k  & 0 & - 1 \\
    \gamma^k & 1 & \frac{1}{2}
    \end{array}

****************
Ralston's method
****************

.. math::

    \alpha = \frac{2}{3}
    \Rightarrow
    \begin{array}{c|cc}
    k & 0 & 1 \\
    \hline
    \beta^k  & 0 & - \frac{5}{9} \\
    \gamma^k & \frac{2}{3} & \frac{3}{4}
    \end{array}

