For a sequence of :math:`N` complex numbers :math:`z_n` with :math:`\seq{n}{N - 1}`, the discrete Fourier transform is defined as

.. math::

    Z_k
    \equiv
    \sum_{n = 0}^{N - 1}
    z_n
    \twiddle{- 2 \pi}{n k}{N}
    \equiv
    \rdft{N}{z}{0}{1}{N - 1},

where :math:`\seq{k}{N - 1}`.

Assuming that :math:`N` is a multiple of :math:`2`, we decompose the sum into two components:

.. math::

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

to yield

.. math::

    Z_k
    =
    \rdft{N / 2}{z}{0}{2}{N - 2}
    +
    \twiddle{- 2 \pi}{k}{N}
    \rdft{N / 2}{z}{1}{3}{N - 1},

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
    \rdft{N / 2}{z}{0}{2}{N - 2}
    -
    \twiddle{- 2 \pi}{k}{N}
    \rdft{N / 2}{z}{1}{3}{N - 1},

we obtain the following relation known as the decimation in time:

.. math::

    Z_k
    =
    \rdft{N / 2}{z}{0}{2}{N - 2}
    +
    \twiddle{- 2 \pi}{k}{N}
    \rdft{N / 2}{z}{1}{3}{N - 1},

    Z_{k + N / 2}
    =
    \rdft{N / 2}{z}{0}{2}{N - 2}
    -
    \twiddle{- 2 \pi}{k}{N}
    \rdft{N / 2}{z}{1}{3}{N - 1},

with :math:`\seq{k}{N / 2 - 1}`.

The inverse transform

.. math::

    \mathcal{F}_N^{-1}: \mathbb{C}^N \rightarrow \mathbb{C}^N

is defined as an identical way with the opposite sign of the twiddle factor (and the pre-factor :math:`N`):

.. math::

    z_n
    \equiv
    \frac{1}{N}
    \sum_{k = 0}^{N - 1}
    Z_k
    \twiddle{2 \pi}{n k}{N}
    \equiv
    \irdft{N}{Z}{0}{1}{N - 1}

with :math:`\seq{n}{N - 1}`, and equivalently we have

.. math::

    z_n
    =
    \irdft{N / 2}{Z}{0}{2}{N - 2}
    +
    \twiddle{2 \pi}{n}{N}
    \irdft{N / 2}{Z}{1}{3}{N - 1},

    z_{n + N / 2}
    =
    \irdft{N / 2}{Z}{0}{2}{N - 2}
    -
    \twiddle{2 \pi}{n}{N}
    \irdft{N / 2}{Z}{1}{3}{N - 1},

with :math:`\seq{n}{N / 2 - 1}`.

