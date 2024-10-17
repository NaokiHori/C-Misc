
.. _mathematical_formulation_fourier_series:

##############
Fourier series
##############

We consider a one-dimensional periodic signal :math:`f \left( x \right) \in \mathbb{C}` with :math:`x \in \left[ 0, L \right)`.
The Fourier series expansion of this signal is given by

.. math::

    f \left( x \right)
    =
    \sum_{k = - \infty}^{\infty}
    F_k
    \twiddle{2 \pi}{k x}{L},

where :math:`k \in \mathbb{Z}`, :math:`F_k \in \mathbb{C}`, and :math:`I` is the imaginary unit :math:`\sqrt{-1}`.
A weighted average of this relation in the given range:

.. math::

    \frac{1}{L}
    \int_{0}^{L}
    f \left( x \right)
    \twiddle{- 2 \pi}{k x}{L}
    dx

yields

.. math::

    &
    \frac{1}{L}
    \int_{0}^{L}
    \sum_{l = - \infty}^{\infty}
    F_l
    \twiddle{2 \pi}{l x}{L}
    \twiddle{- 2 \pi}{k x}{L}
    dx

    =
    &
    \frac{1}{L}
    \sum_{l = - \infty}^{\infty}
    F_l
    \int_{0}^{L}
    \twiddle{2 \pi}{\left( l - k \right) x}{L}
    dx
    \,\,
    \left(
        \text{N.B. integral and summation are interchanged}
    \right)

    =
    &
    \sum_{l = - \infty}^{\infty}
    F_l
    \delta_{lk}

    =
    &
    F_k,

which is used to find :math:`F_k` from :math:`f \left( x \right)`.
Here we utilize the periodicity:

.. math::

    \frac{1}{L}
    \int_{0}^{L}
    \twiddle{2 \pi}{n x}{L}
    dx
    =
    \delta_{n0}

with :math:`n \in \mathbb{Z}`.

