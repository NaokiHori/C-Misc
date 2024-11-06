For a sequence of :math:`N` complex numbers :math:`z_n` with :math:`\seq{n}{0}{1}{N - 1}`, the discrete Fourier transform is defined as

.. math::

    Z_k
    \equiv
    \sum_{n = 0}^{N - 1}
    z_n
    \twiddle{- 2 \pi}{n k}{N}
    \equiv
    \dft{k}{N}{z_0}{z_1}{z_{N - 1}},

where :math:`\seq{k}{0}{1}{N - 1}`.

Assuming that :math:`N` is a multiple of :math:`2`, we obtain the following relation which is known as the decimation in time:

.. math::

    \begin{pmatrix}
        Z_{k        } \\
        Z_{k + N / 2} \\
    \end{pmatrix}
    =
    \begin{pmatrix}
        1 &   1 \\
        1 & - 1 \\
    \end{pmatrix}
    \begin{pmatrix}
        1 & 0 \\
        0 & \twiddle{- 2 \pi}{k}{N} \\
    \end{pmatrix}
    \begin{pmatrix}
        \dft{k}{N / 2}{z_0}{z_2}{z_{N - 2}} \\
        \dft{k}{N / 2}{z_1}{z_3}{z_{N - 1}} \\
    \end{pmatrix}

for :math:`\seq{k}{0}{1}{N / 2 - 1}`.

.. derivation_disclosure::

    We decompose the sum into the even and odd components:

    .. math::

        Z_k
        =
        &
        \sum_{n = 0}^{N / 2 - 1}
        z_{2 n}
        \twiddle{- 2 \pi}{2 n k}{N}
        +
        \sum_{n = 0}^{N / 2 - 1}
        z_{2 n + 1}
        \twiddle{- 2 \pi}{\left( 2 n + 1 \right) k}{N}

        =
        &
        \sum_{n = 0}^{N / 2 - 1}
        z_{2 n}
        \twiddle{- 2 \pi}{n k}{N / 2}
        +
        \twiddle{- 2 \pi}{k}{N}
        \sum_{n = 0}^{N / 2 - 1}
        z_{2 n + 1}
        \twiddle{- 2 \pi}{n k}{N / 2}

        =
        &
        \dft{k}{N / 2}{z_0}{z_2}{z_{N - 2}}
        +
        \twiddle{- 2 \pi}{k}{N}
        \dft{k}{N / 2}{z_1}{z_3}{z_{N - 1}},

    where :math:`\seq{k}{0}{1}{N - 1}`.
    Assigning :math:`k \leftarrow k + N / 2` to this relation yields

    .. math::

        Z_{k + N / 2}
        =
        &
        \sum_{n = 0}^{N / 2 - 1}
        z_{2 n}
        \twiddle{- 2 \pi}{n \left( k + N / 2 \right)}{N / 2}
        +
        \twiddle{- 2 \pi}{\left( k + N / 2 \right)}{N}
        \sum_{n = 0}^{N / 2 - 1}
        z_{2 n + 1}
        \twiddle{- 2 \pi}{n \left( k + N / 2 \right)}{N / 2}

        =
        &
        \sum_{n = 0}^{N / 2 - 1}
        z_{2 n}
        \twiddle{- 2 \pi}{n k}{N / 2}
        -
        \twiddle{- 2 \pi}{k}{N}
        \sum_{n = 0}^{N / 2 - 1}
        z_{2 n + 1}
        \twiddle{- 2 \pi}{n k}{N / 2}

        =
        &
        \dft{k}{N / 2}{z_0}{z_2}{z_{N - 2}}
        -
        \twiddle{- 2 \pi}{k}{N}
        \dft{k}{N / 2}{z_1}{z_3}{z_{N - 1}},

    indicating that considering :math:`\seq{k}{0}{1}{N / 2 - 1}` is sufficient to obtain :math:`Z_k` for :math:`\left( \seq{k}{0}{1}{N - 1} \right)`.

Similarly, when :math:`N` is a multiple of :math:`3`, we have

.. math::

    \begin{pmatrix}
        Z_{k          } \\
        Z_{k +   N / 3} \\
        Z_{k + 2 N / 3} \\
    \end{pmatrix}
    =
    \begin{pmatrix}
        1
        &
        1
        &
        1
        \\
        1
        &
        \twiddle{- 2 \pi}{1}{3}
        &
        \twiddle{2 \pi}{1}{3}
        \\
        1
        &
        \twiddle{2 \pi}{1}{3}
        &
        \twiddle{- 2 \pi}{1}{3}
        \\
    \end{pmatrix}
    \begin{pmatrix}
        1
        &
        0
        &
        0
        \\
        0
        &
        \twiddle{- 2 \pi}{k}{N}
        &
        0
        \\
        0
        &
        0
        &
        \twiddle{- 2 \pi}{2 k}{N}
        \\
    \end{pmatrix}
    \begin{pmatrix}
        \dft{k}{N / 3}{z_0}{z_3}{z_{N - 3}} \\
        \dft{k}{N / 3}{z_1}{z_4}{z_{N - 2}} \\
        \dft{k}{N / 3}{z_2}{z_5}{z_{N - 1}} \\
    \end{pmatrix}

for :math:`\seq{k}{0}{1}{N / 3 - 1}`.

