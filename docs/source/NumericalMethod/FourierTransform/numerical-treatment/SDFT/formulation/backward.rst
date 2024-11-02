The inverse transform is defined as

.. math::

    z_n
    =
    \frac{1}{N}
    \sum_{k = 0}^{N - 1}
    Z_k
    \twiddle{2 \pi}{\left( n + \frac{1}{2} \right) k}{N}
    \equiv
    \idft{n}{N}{Z_0}{Z_1}{Z_{N - 1}}.

.. note::

   Due to

   .. math::

        z_n
        =
        \frac{1}{N}
        \sum_{k = 0}^{N - 1}
        \left(
            Z_k
            \exp \left( \frac{\pi k}{N} I \right)
        \right)
        \twiddle{2 \pi}{n k}{N},

   the inverse transform of a shifted signal can be computed using the normal inverse discrete Fourier transform where the input signal is multiplied by a pre-factor beforehand.

By utilizing the decimation in time, the inverse transform leads to

.. math::

    \begin{pmatrix}
        z_{n        } \\
        z_{n + N / 2} \\
    \end{pmatrix}
    =
    \begin{pmatrix}
        \frac{1}{2} &   \frac{1}{2} \twiddle{\pi}{2 n + 1}{N} \\
        \frac{1}{2} & - \frac{1}{2} \twiddle{\pi}{2 n + 1}{N} \\
    \end{pmatrix}
    \begin{pmatrix}
        \idft{n}{N / 2}{Z_0}{Z_2}{Z_{N - 2}} \\
        \idft{n}{N / 2}{Z_1}{Z_3}{Z_{N - 1}} \\
    \end{pmatrix}

for :math:`\seq{n}{0}{1}{N / 2 - 1}`.

