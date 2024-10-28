We define the shifted discrete Fourier transform as

.. math::

    Z_k
    \equiv
    \sum_{n = 0}^{N - 1}
    z_n
    \twiddle{- 2 \pi}{\left( n + \frac{1}{2} \right) k}{N}
    \equiv
    \dft{k}{N}{z_0}{z_1}{z_{N - 1}},

where :math:`\seq{k}{N - 1}`.

.. note::

    Since this relation leads to

    .. math::

        Z_k
        =
        \twiddle{- \pi}{k}{N}
        \sum_{n = 0}^{N - 1}
        z_n
        \twiddle{- 2 \pi}{n k}{N},

    the shifted transform can be computed using the normal discrete Fourier transform followed by multiplication by a pre-factor.

By utilizing the decimation in time, the forward transform leads to

.. math::

    \begin{pmatrix}
        Z_{k        } \\
        Z_{k + N / 2} \\
    \end{pmatrix}
    =
    \begin{pmatrix}
            \twiddle{\pi}{k}{N} &   \twiddle{- \pi}{k}{N} \\
        - I \twiddle{\pi}{k}{N} & I \twiddle{- \pi}{k}{N} \\
    \end{pmatrix}
    \begin{pmatrix}
        \dft{k}{N / 2}{z_0}{z_2}{z_{N - 2}} \\
        \dft{k}{N / 2}{z_1}{z_3}{z_{N - 1}} \\
    \end{pmatrix}

for :math:`\seq{k}{N / 2 - 1}`.

