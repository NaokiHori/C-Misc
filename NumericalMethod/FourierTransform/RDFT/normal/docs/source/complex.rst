#####################################
Fourier transform of a complex signal
#####################################

For a sequence of complex numbers :math:`z_n` where :math:`\seq{n}{N - 1}` with :math:`N` being the degree of freedom, its discrete Fourier transform is defined as

.. math::

    Z_k
    \equiv
    \sum_{n = 0}^{N - 1}
    z_n
    \twiddle{-}{n k}{N}
    \equiv
    \fourier{N}{z}{0}{1}{N - 1},

where :math:`\seq{k}{N - 1}`.

Assuming that :math:`N` is a multiple of :math:`2`, we decompose the right-hand side into two components:

.. math::

    &
    \sum_{n = 0}^{N / 2 - 1}
    z_{2 n}
    \twiddle{-}{2 n k}{N}
    +
    \sum_{n = 0}^{N / 2 - 1}
    z_{2 n + 1}
    \twiddle{-}{\left( 2 n + 1 \right) k}{N}

    =
    &
    \sum_{n = 0}^{N / 2 - 1}
    z_{2 n}
    \twiddle{-}{n k}{N / 2}
    +
    \twiddle{-}{k}{N}
    \sum_{n = 0}^{N / 2 - 1}
    z_{2 n + 1}
    \twiddle{-}{n k}{N / 2}

to yield

.. math::

    Z_k
    =
    \fourier{N / 2}{z}{0}{2}{N / 2 - 2}
    +
    \twiddle{-}{k}{N}
    \fourier{N / 2}{z}{1}{3}{N / 2 - 1},

where :math:`\seq{k}{N - 1}`.

This relation is known as the decimation in time (`DIT`).

While :math:`Z_k` with :math:`k < N / 2` are computed following the above relation, :math:`Z_k` with :math:`N / 2 \le k` are simply obtained by

.. math::

    Z_{k + N / 2}
    =
    &
    \sum_{n = 0}^{N / 2 - 1}
    z_{2 n}
    \twiddle{-}{n \left( k + N / 2 \right)}{N / 2}
    +
    \twiddle{-}{\left( k + N / 2 \right)}{N}
    \sum_{n = 0}^{N / 2 - 1}
    z_{2 n + 1}
    \twiddle{-}{n \left( k + N / 2 \right)}{N / 2}

    =
    &
    \sum_{n = 0}^{N / 2 - 1}
    z_{2 n}
    \twiddle{-}{n k}{N / 2}
    -
    \twiddle{-}{k}{N}
    \sum_{n = 0}^{N / 2 - 1}
    z_{2 n + 1}
    \twiddle{-}{n k}{N / 2}

    =
    &
    \fourier{N / 2}{z}{0}{2}{N / 2 - 2}
    -
    \twiddle{-}{k}{N}
    \fourier{N / 2}{z}{1}{3}{N / 2 - 1}.

To summarize,

.. math::

    Z_k
    =
    \fourier{N / 2}{z}{0}{2}{N / 2 - 2}
    +
    \twiddle{-}{k}{N}
    \fourier{N / 2}{z}{1}{3}{N / 2 - 1},

    Z_{k + N / 2}
    =
    \fourier{N / 2}{z}{0}{2}{N / 2 - 2}
    -
    \twiddle{-}{k}{N}
    \fourier{N / 2}{z}{1}{3}{N / 2 - 1},

with :math:`\seq{k}{N / 2 - 1}`.

The inverse transform is defined as an identical way except the sign of the twiddle factor:

.. math::

    z_n
    \equiv
    \sum_{k = 0}^{N - 1}
    Z_k
    \twiddle{+}{n k}{N}
    \equiv
    \ifourier{N}{Z}{0}{1}{N - 1}

with :math:`\seq{n}{N - 1}`, and we have

.. math::

    z_n
    =
    \ifourier{N / 2}{Z}{0}{2}{N / 2 - 2}
    +
    \twiddle{+}{k}{N}
    \ifourier{N / 2}{Z}{1}{3}{N / 2 - 1},

    z_{n + N / 2}
    =
    \ifourier{N / 2}{Z}{0}{2}{N / 2 - 2}
    -
    \twiddle{+}{k}{N}
    \ifourier{N / 2}{Z}{1}{3}{N / 2 - 1},

with :math:`\seq{n}{N / 2 - 1}`.

