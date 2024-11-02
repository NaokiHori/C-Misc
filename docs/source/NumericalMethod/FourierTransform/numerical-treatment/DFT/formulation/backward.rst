The inverse transform is defined as an identical way with the opposite sign of the twiddle factor (and the pre-factor :math:`1 / N`):

.. math::

    z_n
    \equiv
    \frac{1}{N}
    \sum_{k = 0}^{N - 1}
    Z_k
    \twiddle{2 \pi}{n k}{N}
    \equiv
    \idft{n}{N}{Z_0}{Z_1}{Z_{N - 1}}

for :math:`\seq{n}{0}{1}{N - 1}`, and similarly we can deduce

.. math::

    \begin{pmatrix}
        z_n \\
        z_{n + N / 2} \\
    \end{pmatrix}
    =
    \begin{pmatrix}
        \frac{1}{2} &   \frac{1}{2} \twiddle{2 \pi}{n}{N} \\
        \frac{1}{2} & - \frac{1}{2} \twiddle{2 \pi}{n}{N} \\
    \end{pmatrix}
    \begin{pmatrix}
        \idft{n}{N / 2}{Z_0}{Z_2}{Z_{N - 2}} \\
        \idft{n}{N / 2}{Z_1}{Z_3}{Z_{N - 1}} \\
    \end{pmatrix},

for :math:`\seq{n}{0}{1}{N / 2 - 1}`.

.. derivation_disclosure::

    The even-odd decomposition yields

    .. math::

        \idft{n}{N}{Z_0}{Z_1}{Z_{N - 1}}
        &
        =
        \frac{1}{N}
        \sum_{k = 0}^{N - 1}
        Z_k
        \twiddle{2 \pi}{n k}{N}

        &
        =
        \frac{1}{N}
        \sum_{k = 0}^{N / 2 - 1}
        Z_{2 k}
        \twiddle{2 \pi}{n \left( 2 k \right)}{N}
        +
        \frac{1}{N}
        \sum_{k = 0}^{N / 2 - 1}
        Z_{2 k + 1}
        \twiddle{2 \pi}{n \left( 2 k + 1 \right)}{N}

        &
        =
        \frac{1}{2}
        \frac{1}{N / 2}
        \sum_{k = 0}^{N / 2 - 1}
        Z_{2 k}
        \twiddle{2 \pi}{n k}{N / 2}
        +
        \frac{1}{2}
        \twiddle{2 \pi}{n}{N}
        \frac{1}{N / 2}
        \sum_{k = 0}^{N / 2 - 1}
        Z_{2 k + 1}
        \twiddle{2 \pi}{n k}{N / 2}

        &
        =
        \frac{1}{2}
        \idft{n}{N / 2}{Z_0}{Z_2}{Z_{N - 2}}
        +
        \frac{1}{2}
        \twiddle{2 \pi}{n}{N}
        \idft{n}{N / 2}{Z_1}{Z_3}{Z_{N - 1}}.

    Assigning :math:`n \leftarrow n + N / 2` yields

    .. math::

        \idft{n + N / 2}{N}{Z_0}{Z_1}{Z_{N - 1}}
        &
        =
        \frac{1}{2}
        \frac{1}{N / 2}
        \sum_{k = 0}^{N / 2 - 1}
        Z_{2 k}
        \twiddle{2 \pi}{\left( n + N / 2 \right) k}{N / 2}
        +
        \frac{1}{2}
        \twiddle{2 \pi}{n + N / 2}{N}
        \frac{1}{N / 2}
        \sum_{k = 0}^{N / 2 - 1}
        Z_{2 k + 1}
        \twiddle{2 \pi}{\left( n + N / 2 \right) k}{N / 2}

        &
        =
        \frac{1}{2}
        \frac{1}{N / 2}
        \sum_{k = 0}^{N / 2 - 1}
        Z_{2 k}
        \twiddle{2 \pi}{n k}{N / 2}
        \exp \left( 2 \pi k I \right)
        +
        \frac{1}{2}
        \twiddle{2 \pi}{n}{N}
        \exp \left( \pi I \right)
        \frac{1}{N / 2}
        \sum_{k = 0}^{N / 2 - 1}
        Z_{2 k + 1}
        \twiddle{2 \pi}{n k}{N / 2}
        \exp \left( 2 \pi k I \right)

        &
        =
        \frac{1}{2}
        \frac{1}{N / 2}
        \sum_{k = 0}^{N / 2 - 1}
        Z_{2 k}
        \twiddle{2 \pi}{n k}{N / 2}
        -
        \frac{1}{2}
        \twiddle{2 \pi}{n}{N}
        \frac{1}{N / 2}
        \sum_{k = 0}^{N / 2 - 1}
        Z_{2 k + 1}
        \twiddle{2 \pi}{n k}{N / 2}

        &
        =
        \frac{1}{2}
        \idft{n}{N / 2}{Z_0}{Z_2}{Z_{N - 2}}
        -
        \frac{1}{2}
        \twiddle{2 \pi}{n}{N}
        \idft{n}{N / 2}{Z_1}{Z_3}{Z_{N - 1}}.

