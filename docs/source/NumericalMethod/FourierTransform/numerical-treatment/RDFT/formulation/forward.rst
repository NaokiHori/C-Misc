We aim at finding the discrete Fourier transform of a sequence composed of :math:`N` real numbers :math:`x_n`:

.. math::

    X_k
    =
    \rdft{N}{x}{0}{1}{N - 1}
    =
    \sum_{n = 0}^{N - 1}
    x_n
    \twiddle{- 2 \pi}{n k}{N},

where :math:`\seq{k}{N - 1}`.

By utilizing the decimation in time (assuming :math:`N` is a multiple of :math:`2`), we have

.. math::

    X_k
    =
    \rdft{N / 2}{x}{0}{2}{N - 2}
    +
    \twiddle{- 2 \pi}{k}{N}
    \rdft{N / 2}{x}{1}{3}{N - 1}.

Although this relation is fulfilled for :math:`\seq{k}{N - 1}`, it is enough to consider up to :math:`k = N / 2` as :math:`X_{N - k} = X_k^*` is satisfied.

.. mydetails:: Derivation

    .. math::

        X_{N - k}
        &
        =
        \sum_{n = 0}^{N - 1}
        x_n
        \twiddle{- 2 \pi}{n \left( N - k \right)}{N}

        &
        =
        \sum_{n = 0}^{N - 1}
        x_n
        \exp \left( - 2 \pi n I \right)
        \twiddle{2 \pi}{n k}{N}

        &
        =
        \sum_{n = 0}^{N - 1}
        x_n
        \twiddle{2 \pi}{n k}{N}

        &
        =
        \left[
            \sum_{n = 0}^{N - 1}
            x_n
            \twiddle{- 2 \pi}{n k}{N}
        \right]^*
        \,\, \left( \because x_n^* = x_n \right)

        &
        =
        X_k^*.

For later notational convenience, we introduce

.. math::

    X_k^e
    &
    \equiv
    \rdft{N / 2}{x}{0}{2}{N - 2},

    X_k^o
    &
    \equiv
    \rdft{N / 2}{x}{1}{3}{N - 1},

and the relation of interest here is written as

.. math::

    X_k
    =
    X_k^e
    +
    \twiddle{- 2 \pi}{k}{N}
    X_k^o

with :math:`\seq{k}{N / 2}`.

.. note::

    It is readily apparent that :math:`X_0` and :math:`X_{N / 2}` are real.

These discrete Fourier transforms of real sequence can be computed at once using the fact that the imaginary parts are all zero.
Specifically, we consider a signal :math:`z_n` composed of :math:`N / 2` complex numbers:

.. math::

    z_n
    =
    x_{2 n}
    +
    I
    x_{2 n + 1},

where :math:`\seq{n}{N / 2 - 1}`.

By using the complex conjugate:

.. math::

    z_n^*
    =
    x_{2 n}
    -
    I
    x_{2 n + 1},

we have

.. math::

    x_{2 n    } &= + \frac{1}{2} z_n + \frac{1}{2} z_n^*,

    x_{2 n + 1} &= - \frac{I}{2} z_n + \frac{I}{2} z_n^*,

where :math:`\seq{n}{N / 2 - 1}`.

Their discrete Fourier transforms lead to

.. math::

    X_k^e
    &
    =
    +
    \frac{1}{2}
    \rdft{N / 2}{z}{0}{1}{N / 2 - 1}
    +
    \frac{1}{2}
    \rdft{N / 2}{z^*}{0}{1}{N / 2 - 1},

    X_k^o
    &
    =
    -
    \frac{I}{2}
    \rdft{N / 2}{z}{0}{1}{N / 2 - 1}
    +
    \frac{I}{2}
    \rdft{N / 2}{z^*}{0}{1}{N / 2 - 1},

where :math:`\seq{k}{N / 2 - 1}`.

.. note::

    :math:`k = N / 2` will be discussed later.

We do not need to calculate the discrete Fourier transform of :math:`z_n^*`:

.. math::

    \sum_{n = 0}^{N / 2 - 1}
    z_n^*
    \twiddle{- 2 \pi}{n k}{N / 2}
    =
    \rdft{N / 2}{z^*}{0}{1}{N / 2 - 1}

since this is equal to

.. math::

    Z_{N / 2 - k}^*.

.. mydetails:: Derivation

    .. math::

        \sum_{n = 0}^{N / 2 - 1}
        z_n^*
        \twiddle{- 2 \pi}{n k}{N / 2}
        =
        &
        \sum_{n = 0}^{N / 2 - 1}
        z_n^*
        \left[ \twiddle{- 2 \pi}{n \left( -k \right)}{N / 2} \right]^*

        =
        &
        \sum_{n = 0}^{N / 2 - 1}
        z_n^*
        \left[ \twiddle{- 2 \pi}{n \left( -k \right)}{N / 2} \right]^*
        \left[ \twiddle{- 2 \pi}{n N / 2}{N / 2} \right]^*
        \,\,
        \left(
            \because
            \left[ \twiddle{- 2 \pi}{n N / 2}{N / 2} \right]^*
            \equiv
            1
        \right)

        =
        &
        \sum_{n = 0}^{N / 2 - 1}
        z_n^*
        \left[
            \twiddle{- 2 \pi}{n \left( -k \right)}{N / 2}
            \twiddle{- 2 \pi}{n N / 2}{N / 2}
        \right]^*

        =
        &
        \sum_{n = 0}^{N / 2 - 1}
        z_n^*
        \left[
            \twiddle{- 2 \pi}{n \left( N / 2 - k \right)}{N / 2}
        \right]^*

        =
        &
        \left[
            \sum_{n = 0}^{N / 2 - 1}
            z_n
            \twiddle{- 2 \pi}{n \left( N / 2 - k \right)}{N / 2}
        \right]^*
        \,\,
        \left(
            \because
            \left( a b \right)^*
            \equiv
            a^* b^*
        \right)

        =
        &
        Z_{N / 2 - k}^*.

Thus we have

.. math::

    X_k^e
    &
    =
    +
    \frac{1}{2}
    Z_k
    +
    \frac{1}{2}
    Z_{N / 2 - k}^*,

    X_k^o
    &
    =
    -
    \frac{I}{2}
    Z_k
    +
    \frac{I}{2}
    Z_{N / 2 - k}^*,

where :math:`\seq{k}{N / 2 - 1}`.

Note that, to evaluate :math:`X_0^e` and :math:`X_0^o` using this relation, we need :math:`Z_{N / 2}^*`.
Although this is undefined as :math:`Z_k` is available up to :math:`k = N / 2 - 1`, following the definition, we see:

.. math::

    Z_{N / 2}^*
    =
    \sum_{n = 0}^{N / 2 - 1}
    z_n^*
    =
    \left(
        \sum_{n = 0}^{N / 2 - 1}
        z_n
    \right)^*
    =
    Z_0^*,

and accordingly

.. math::

    X_0^e
    &
    =
    +
    \frac{1}{2}
    Z_0
    +
    \frac{1}{2}
    Z_0^*,

    X_0^o
    &
    =
    -
    \frac{I}{2}
    Z_0
    +
    \frac{I}{2}
    Z_0^*.

Similarly, we have

.. math::

    X_{N / 2}^e
    &
    =
    +
    \frac{1}{2}
    Z_0
    +
    \frac{1}{2}
    Z_0^*,

    X_{N / 2}^o
    &
    =
    -
    \frac{I}{2}
    Z_0
    +
    \frac{I}{2}
    Z_0^*,

from which we compute the resulting relation

.. math::

    X_k
    =
    X_k^e
    +
    \twiddle{- 2 \pi}{k}{N}
    X_k^o

with :math:`\seq{k}{N / 2}`.

