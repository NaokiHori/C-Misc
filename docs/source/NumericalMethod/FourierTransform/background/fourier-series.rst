We consider a one-dimensional periodic signal :math:`f \left( x \right)` with :math:`x \in \left[ 0, L \right)`, whose Fourier series expansion is given by

.. math::

    f \left( x \right)
    =
    \sum_{k = - \infty}^{\infty}
    F_k
    \exp
    \left(
        \frac{
            2 \pi k x
        }{
            L
        }
        I
    \right),

where :math:`I` is the imaginary unit and :math:`k \in \mathbb{Z}`.

By utilizing the identity due to the orthogonality of the trigonometric functions:

.. math::

    \frac{1}{L}
    \int_{0}^{L}
    \exp
    \left(
        \frac{
            2 \pi n x
        }{
            L
        }
        I
    \right)
    dx
    =
    \delta_{n0}

with :math:`n \in \mathbb{Z}`, we have

.. math::

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
    =
    \frac{1}{L}
    \int_{0}^{L}
    \sum_{l = - \infty}^{\infty}
    F_l
    \exp
    \left(
        \frac{
            2 \pi l x
        }{
            L
        }
        I
    \right)
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
    =
    \frac{1}{L}
    \sum_{l = - \infty}^{\infty}
    F_l
    \int_{0}^{L}
    \exp
    \left\{
        \frac{
            2 \pi \left( l - k \right) x
        }{
            L
        }
        I
    \right\}
    dx
    \,\,
    \left(
        \text{N.B. integral and summation are interchanged}
    \right)

    &
    =
    \sum_{l = - \infty}^{\infty}
    F_l
    \delta_{lk}

    &
    =
    F_k,

giving a way to compute Fourier coefficient :math:`F_k`.

