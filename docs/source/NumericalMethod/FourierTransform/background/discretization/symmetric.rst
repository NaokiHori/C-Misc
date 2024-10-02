##############
Symmetric case
##############

We can prescribe the sampling points differently:

.. math::

    x_n
    =
    \frac{
        2 n
        +
        1
    }{
        2
    }
    \frac{
        L
    }{
        N
    },

giving

.. math::

    F_k
    &
    =
    \frac{1}{N}
    \sum_{n = 0}^{N - 1}
    f_n
    \exp
    \left(
        -
        \frac{
            2 \pi k x_n
        }{
            L
        }
        I
    \right)

    &
    =
    \frac{1}{N}
    \sum_{n = 0}^{N - 1}
    f_n
    \exp
    \left\{
        -
        \frac{
            2 \pi k \left( n + \frac{1}{2} \right)
        }{
            N
        }
        I
    \right\},

and

.. math::

    f_n
    =
    \sum_{k = 0}^{N - 1}
    F_k
    \exp
    \left\{
        \frac{
            2 \pi k \left( n + \frac{1}{2} \right)
        }{
            N
        }
        I
    \right\}.

For later convenience, we substitute :math:`N \rightarrow 2 N`, yielding

.. math::

    F_k
    &
    =
    \frac{1}{2 N}
    \sum_{n = 0}^{2 N - 1}
    f_n
    \exp
    \left\{
        -
        \frac{
            \pi k \left( n + \frac{1}{2} \right)
        }{
            N
        }
        I
    \right\},

    f_n
    &
    =
    \sum_{k = 0}^{2 N - 1}
    F_k
    \exp
    \left\{
        \frac{
            \pi k \left( n + \frac{1}{2} \right)
        }{
            N
        }
        I
    \right\}.

.. toctree::

    even.rst
    odd.rst

