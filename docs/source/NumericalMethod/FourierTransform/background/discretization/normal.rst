###########
Normal case
###########

Now we consider the corresponding discrete relation:

.. math::

    F_k
    &
    =
    \frac{1}{L}
    \int_{0}^{L}
    f \left( x \right)
    \exp
    \left(
        -
        \frac{
            2 \pi k x
        }{
            L
        }
        I
    \right)
    dx

    &
    \approx
    \frac{1}{L}
    \int_{0}^{L}
    \sum_{n = 0}^{N - 1}
    f_n
    \delta
    \left(
        x
        -
        x_n
    \right)
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
    dx

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
    \,\,
    \left(
        \text{N.B. integral and summation are interchanged}
    \right).

Namely, the continuous function :math:`f \left( x \right)` is represented by the superposition of :math:`N` Dirac delta functions, which have non-zero values at :math:`x_n \in \left[ 0, L \right)`.

By taking the sampling points as

.. math::

    x_n
    =
    n
    \frac{
        L
    }{
        N
    },

we obtain

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
    \left(
        -
        \frac{
            2 \pi k n
        }{
            N
        }
        I
    \right).

Correspondingly, we notice

.. math::

    \sum_{k = 0}^{N - 1}
    F_k
    \exp
    \left(
        \frac{
            2 \pi k m
        }{
            N
        }
        I
    \right)
    &
    =
    \sum_{k = 0}^{N - 1}
    \frac{1}{N}
    \sum_{n = 0}^{N - 1}
    f_n
    \exp
    \left(
        -
        \frac{
            2 \pi k n
        }{
            N
        }
        I
    \right)
    \exp
    \left(
        \frac{
            2 \pi k m
        }{
            N
        }
        I
    \right)

    &
    =
    \frac{1}{N}
    \sum_{n = 0}^{N - 1}
    \sum_{k = 0}^{N - 1}
    f_n
    \exp
    \left\{
        \frac{
            2 \pi k \left( m - n \right)
        }{
            N
        }
        I
    \right\}

    &
    =
    \frac{1}{N}
    \sum_{n = 0}^{N - 1}
    f_n
    N
    \delta_{mn}

    &
    =
    f_m,

where we use

.. math::

    \sum_{k = 0}^{N - 1}
    \exp
    \left(
        \frac{
            2 \pi k n
        }{
            N
        }
    \right)
    =
    N
    \delta_{n0}

which can be derived by adopting the identity (for :math:`n \ne 0`):

.. math::

    \sum_{k = 0}^{N - 1}
    \exp \left( n k I \right)
    =
    \frac{
        1 - \exp \left( n N I \right)
    }{
        1 - \exp \left( n I \right)
    }.

In summary, we confirm the discrete Fourier transform and its inverse relation:

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
            2 \pi k n
        }{
            N
        }
        I
    \right),

    f_n
    &
    =
    \sum_{k = 0}^{N - 1}
    F_k
    \exp
    \left(
        \frac{
            2 \pi k n
        }{
            N
        }
        I
    \right),

with

.. math::

    x_n
    =
    n
    \frac{
        L
    }{
        N
    }.

