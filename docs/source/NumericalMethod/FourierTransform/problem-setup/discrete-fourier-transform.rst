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

Namely, the continuous function :math:`f \left( x \right)` is represented by the superposition of :math:`N` Dirac delta functions, which have non-zero value at :math:`x_n \in \left[ 0, L \right)`.

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
            2 \pi k n
        }{
            N
        }
        I
    \right)
    &
    =
    \sum_{k = 0}^{N - 1}
    \left\{
        \frac{1}{N}
        \sum_{m = 0}^{N - 1}
        f_m
        \exp
        \left(
            -
            \frac{
                2 \pi k m
            }{
                N
            }
            I
        \right)
    \right\}
    \exp
    \left(
        \frac{
            2 \pi k n
        }{
            N
        }
        I
    \right)

    &
    =
    \frac{1}{N}
    \sum_{m = 0}^{N - 1}
    \sum_{k = 0}^{N - 1}
    f_m
    \exp
    \left\{
        \frac{
            2 \pi k \left( n - m \right)
        }{
            N
        }
        I
    \right\}

    &
    =
    \frac{1}{N}
    \sum_{m = 0}^{N - 1}
    f_m
    N
    \delta_{nm}

    &
    =
    f_n,

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
        I
    \right)
    =
    N
    \delta_{n0},

which can be derived by adopting the identity (for :math:`n \ne 0`):

.. math::

    \sum_{k = 0}^{N - 1}
    \exp
    \left(
        \frac{2 \pi n}{N} k I
    \right)
    &
    =
    \frac{
        1 - \exp \left( \frac{2 \pi n}{N} N I \right)
    }{
        1 - \exp \left( \frac{2 \pi n}{N} I \right)
    }

    &
    =
    \frac{
        1 - \exp \left( 2 \pi n I \right)
    }{
        1 - \exp \left( \frac{2 \pi n}{N} I \right)
    }

    &
    =
    0.

In summary, the discrete Fourier transform :math:`\mathcal{F}` and its inverse transform :math:`\mathcal{F}^{-1}` are given by:

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

Note that :math:`\mathcal{F}: \mathbb{C}^N \rightarrow \mathbb{C}^N` and :math:`\mathcal{F}^{-1}: \mathbb{C}^N \rightarrow \mathbb{C}^N` are both linear operators.

