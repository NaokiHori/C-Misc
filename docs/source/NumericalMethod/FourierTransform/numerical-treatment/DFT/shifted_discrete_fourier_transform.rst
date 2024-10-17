We define the shifted discrete Fourier transform as

.. math::

    Z_k^{\prime}
    \equiv
    \sum_{n = 0}^{N - 1}
    z_n
    \twiddle{- 2 \pi}{\left( n + \frac{1}{2} \right) k}{N},

where :math:`\seq{k}{N - 1}`.
Due to

.. math::

    \exp \left( - \frac{\pi k}{N} I \right)
    \sum_{n = 0}^{N - 1}
    z_n
    \twiddle{- 2 \pi}{n k}{N}
    =
    \exp \left( - \frac{\pi k}{N} I \right)
    Z_k,

the shifted transform can be computed using the normal discrete Fourier transform, followed by multiplication by a pre-factor.

The inverse transform

.. math::

    z_n
    =
    \frac{1}{N}
    \sum_{k = 0}^{N - 1}
    Z_k^{\prime}
    \twiddle{2 \pi}{\left( n + \frac{1}{2} \right) k}{N}
    =
    \frac{1}{N}
    \sum_{k = 0}^{N - 1}
    \left(
        Z_k^{\prime}
        \exp \left( \frac{\pi k}{N} I \right)
    \right)
    \twiddle{2 \pi}{n k}{N}

can be computed using the normal inverse discrete Fourier transform, where the input signal is multiplied by a pre-factor beforehand.

