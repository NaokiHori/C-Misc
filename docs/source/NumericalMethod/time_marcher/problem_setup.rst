#############
Problem Setup
#############

We aim at solving the initial-value problem (ordinary-differential equation):

.. math::

    \der{p}{t}
    =
    f \left( p, t \right).

As depicted, :math:`f` is not only a function of :math:`p` but also of :math:`t` in general.
Here, however, we limit ourselves to cases when :math:`f` is autonomous:

.. math::

    \der{p}{t}
    =
    f \left( p \right),

which appears frequently in modeling physical phenomena such as Navier-Stokes equations.

We consider its numerical treatment; in particular, we focus on explicit :math:`N`-step Runge-Kutta schemes, which are generally formulated as

.. math::

    & q^0 = p^n

    & \text{do} \quad k = 0, N - 1

    & \quad q^{k + 1} = p^n + \sum_{j = 0}^k a_{k + 1, j} f \left( q^j \right) \Delta t

    & \text{end do}

    & p^{n + 1} = q^N

When implemented naively, this procedure necessitates buffers to store :math:`q`, :math:`f \left( q^0 \right), f \left( q^1 \right), \cdots, f \left( q^{N - 1} \right)`, and :math:`p^n`.
Our primary focus here is to reformulate this into

.. math::

    & q^0 = p^n

    & \text{do} \quad k = 0, N - 1

    & \quad r^k = f \left( q^k \right) + \beta^k r^{k - 1}

    & \quad q^{k + 1} = q^k + \gamma^k r^k \Delta t

    & \text{end do}

    & p^{n + 1} = q^N

such that only two storages (:math:`q^k` and :math:`r^k`) are to be allocated.

Note that Runge-Kutta methods are conveniently expressed using the Butcher tableau:

.. math::

    \begin{array}{c|cc}
    b_0 & a_{0,0} & a_{0,1} & \cdots & a_{0,N - 1} \\
    b_1 & a_{1,0} & a_{1,1} & \cdots & a_{1,N - 1} \\
    \vdots & \vdots & \vdots & \ddots & \vdots \\
    b_{N - 1} & a_{N - 1,0} & a_{N - 1,1} & \cdots & a_{N - 1,N - 1} \\
    \hline
    & a_{N,0} & a_{N,1} & \cdots & a_{N,N - 1} \\
    \end{array}

Our objective is to convert this table into something like

.. math::

    \begin{array}{c|cccc}
    k & 0 & 1 & \cdots & N - 1 \\
    \hline
    \beta^k  & TBA & TBA & \cdots & TBA \\
    \gamma^k & TBA & TBA & \cdots & TBA \\
    \end{array}

