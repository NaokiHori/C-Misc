For a sequence of :math:`N` complex numbers :math:`z_n` with :math:`\seq{n}{N - 1}`, the discrete Fourier transform is defined as

.. math::

    Z_k
    \equiv
    \sum_{n = 0}^{N - 1}
    z_n
    \twiddle{- 2 \pi}{n k}{N}
    \equiv
    \dft{k}{N}{z_0}{z_1}{z_{N - 1}},

where :math:`\seq{k}{N - 1}`.

Assuming that :math:`N` is a multiple of :math:`2`, we decompose the sum into two components:

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

where :math:`\seq{k}{N - 1}`.
Due to

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

we obtain the following relation which is known as the decimation in time:

.. math::

    Z_k
    =
    \dft{k}{N / 2}{z_0}{z_2}{z_{N - 2}}
    +
    \twiddle{- 2 \pi}{k}{N}
    \dft{k}{N / 2}{z_1}{z_3}{z_{N - 1}},

    Z_{k + N / 2}
    =
    \dft{k}{N / 2}{z_0}{z_2}{z_{N - 2}}
    -
    \twiddle{- 2 \pi}{k}{N}
    \dft{k}{N / 2}{z_1}{z_3}{z_{N - 1}},

with :math:`\seq{k}{N / 2 - 1}`.

