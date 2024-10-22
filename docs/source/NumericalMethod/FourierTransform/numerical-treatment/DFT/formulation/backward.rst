The inverse transform is defined as an identical way with the opposite sign of the twiddle factor (and the pre-factor :math:`N`):

.. math::

    z_n
    \equiv
    \frac{1}{N}
    \sum_{k = 0}^{N - 1}
    Z_k
    \twiddle{2 \pi}{n k}{N}
    \equiv
    \idft{n}{N}{Z_0}{Z_1}{Z_{N - 1}}

with :math:`\seq{n}{N - 1}`, and similarly we can deduce the corresponding relation as follows.

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
    \idft{n}{N / 2}{Z_1}{Z_3}{Z_{N - 1}},

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

As a result, the inverse transform is given by

.. math::

    z_n
    =
    \frac{1}{2}
    \idft{n}{N / 2}{Z_0}{Z_2}{Z_{N - 2}}
    +
    \frac{1}{2}
    \twiddle{2 \pi}{n}{N}
    \idft{n}{N / 2}{Z_1}{Z_3}{Z_{N - 1}},

    z_{n + N / 2}
    =
    \frac{1}{2}
    \idft{n}{N / 2}{Z_0}{Z_2}{Z_{N - 2}}
    -
    \frac{1}{2}
    \twiddle{2 \pi}{n}{N}
    \idft{n}{N / 2}{Z_1}{Z_3}{Z_{N - 1}},

for :math:`\seq{n}{N / 2 - 1}`.

