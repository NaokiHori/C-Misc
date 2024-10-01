##########
Derivation
##########

*****************
Forward transform
*****************

Instead of a signal composed of :math:`N` complex numbers, we aim at finding the discrete Fourier transform of a sequence composed of :math:`N` real numbers :math:`x_n`:

.. math::

    X_k
    =
    \rdft{N}{x}{0}{1}{N - 1}
    =
    \sum_{n = 0}^{N - 1}
    x_n
    \rdfttwiddle{-}{n k}{N},

where :math:`\seq{k}{N - 1}`.
Recall that we assume :math:`N` is a multiple of :math:`2`.

By utilizing the `DIT` derived above, we notice

.. math::

    X_k
    &
    =
    \rdft{N / 2}{x}{0}{2}{N / 2 - 2}
    +
    \rdfttwiddle{-}{k}{N}
    \rdft{N / 2}{x}{1}{3}{N / 2 - 1}

    &
    \equiv
    X_k^e
    +
    \rdfttwiddle{-}{k}{N}
    X_k^o

with :math:`\seq{k}{N}`.

Here two discrete Fourier transforms are involved, which can be computed at once as follows since the imaginary parts are all zero.
To start, we create a signal :math:`z_n` composed of :math:`N / 2` complex numbers:

.. math::

    z_n = x_{2 n} + I x_{2 n + 1},

where :math:`\seq{n}{N / 2 - 1}`.

By considering the complex conjugate of them:

.. math::

    z_n^* = x_{2 n} - I x_{2 n + 1},

we notice

.. math::

    x_{2 n    } &= + \frac{1}{2} z_n + \frac{1}{2} z_n^*,

    x_{2 n + 1} &= - \frac{I}{2} z_n + \frac{I}{2} z_n^*,

where :math:`\seq{n}{N / 2 - 1}`.

Their discrete Fourier transforms lead

.. math::

    X_k^e
    \equiv
    \rdft{N / 2}{x}{0}{2}{N / 2 - 2}
    =
    +
    \frac{1}{2}
    \rdft{N / 2}{z}{0}{1}{N / 2 - 1}
    +
    \frac{1}{2}
    \rdft{N / 2}{z^*}{0}{1}{N / 2 - 1},

and

.. math::

    X_k^o
    \equiv
    \rdft{N / 2}{x}{1}{3}{N / 2 - 1}
    =
    -
    \frac{I}{2}
    \rdft{N / 2}{z}{0}{1}{N / 2 - 1}
    +
    \frac{I}{2}
    \rdft{N / 2}{z^*}{0}{1}{N / 2 - 1},

where :math:`\seq{k}{N / 2 - 1}`.

In addition to the discrete Fourier transform of :math:`z_n`:

.. math::

    Z_k
    \equiv
    \rdft{N / 2}{z}{0}{1}{N / 2 - 1}
    =
    \sum_{n = 0}^{N / 2 - 1}
    z_n
    \rdfttwiddle{-}{n k}{N / 2}

with :math:`\seq{k}{N / 2 - 1}`, we seemingly need to evaluate the discrete Fourier transform of :math:`z_n^*`:

.. math::

    \rdft{N / 2}{z^*}{0}{1}{N / 2 - 1},

which is not necessary due to

.. math::

    \rdft{N / 2}{z^*}{0}{1}{N / 2 - 1}
    =
    &
    \sum_{n = 0}^{N / 2 - 1}
    z_n^*
    \rdfttwiddle{-}{n k}{N / 2}

    =
    &
    \sum_{n = 0}^{N / 2 - 1}
    z_n^*
    \left[ \rdfttwiddle{-}{n \left( -k \right)}{N / 2} \right]^*

    =
    &
    \sum_{n = 0}^{N / 2 - 1}
    z_n^*
    \left[ \rdfttwiddle{-}{n \left( -k \right)}{N / 2} \right]^*
    \left[ \rdfttwiddle{-}{n N / 2}{N / 2} \right]^*

    =
    &
    \sum_{n = 0}^{N / 2 - 1}
    z_n^*
    \left[
        \rdfttwiddle{-}{n \left( -k \right)}{N / 2}
        \rdfttwiddle{-}{n N / 2}{N / 2}
    \right]^*

    =
    &
    \sum_{n = 0}^{N / 2 - 1}
    z_n^*
    \left[
        \rdfttwiddle{-}{n \left( N / 2 - k \right)}{N / 2}
    \right]^*

    =
    &
    \left[
        \sum_{n = 0}^{N / 2 - 1}
        z_n
        \rdfttwiddle{-}{n \left( N / 2 - k \right)}{N / 2}
    \right]^*

    =
    &
    \left( Z_{N / 2 - k} \right)^*,

where we adopt the identities

.. math::

    \left( a b \right)^*
    \equiv
    a^* b^*

and

.. math::

    \rdfttwiddle{+}{n N / 2}{N / 2}
    =
    \left[ \rdfttwiddle{-}{n N / 2}{N / 2} \right]^*
    \equiv
    1.

Due to this relation, we notice

.. math::

    X_k^e
    =
    +
    \frac{1}{2}
    Z_k
    +
    \frac{1}{2}
    \left( Z_{N / 2 - k} \right)^*,

and

.. math::

    X_k^o
    =
    -
    \frac{I}{2}
    Z_k
    +
    \frac{I}{2}
    \left( Z_{N / 2 - k} \right)^*,

where :math:`\seq{k}{N / 2 - 1}`.

Finally, we find

.. math::

    X_k
    =
    X_k^e
    +
    \rdfttwiddle{-}{k}{N}
    X_k^o

with :math:`\seq{k}{N / 2 - 1}`.

.. note::

    The case of :math:`k = 0` needs :math:`\left( Z_{N / 2} \right)^*`.
    Although this is undefined as :math:`Z_k` is available up to :math:`k = N / 2 - 1`, following the definition, we see:

    .. math::

        \left( Z_{N / 2} \right)^*
        =
        \sum_{n = 0}^{N / 2 - 1}
        z_n^*
        =
        \left(
            \sum_{n = 0}^{N / 2 - 1}
            z_n
        \right)^*.

    Due to

    .. math::

        Z_0
        =
        \sum_{n = 0}^{N / 2 - 1}
        z_n,

    we see

    .. math::

        \left( Z_{N / 2} \right)^*
        =
        \left( Z_0 \right)^*

    or equivalently

    .. math::

        Z_{N / 2}
        =
        Z_0.

******************
Backward transform
******************

The inverse Fourier transform is obtained essentially by backtracing the forward procedures.

First, from the given input signal:

.. math::

    X_k
    =
    \rdft{N}{x}{0}{1}{N - 1}

with :math:`\seq{k}{N}`, the spectral representations of the even / odd signal are to be recovered.

To this end, we revisit :math:`X_k^e` and :math:`X_k^o`, which are defined as

.. math::

    X_k^e
    &
    \equiv
    +
    \frac{1}{2} Z_k
    +
    \frac{1}{2} \left( Z_{N / 2 - k} \right)^*,

    X_k^o
    &
    \equiv
    -
    \frac{I}{2} Z_k
    +
    \frac{I}{2} \left( Z_{N / 2 - k} \right)^*.

By assigning :math:`k \leftarrow N / 2 - k`, we obtain

.. math::

    X_{N / 2 - k}^e
    &
    =
    +
    \frac{1}{2} Z_{N / 2 - k}
    +
    \frac{1}{2} \left( Z_k \right)^*,

    X_{N / 2 - k}^o
    &
    =
    -
    \frac{I}{2} Z_{N / 2 - k}
    +
    \frac{I}{2} \left( Z_k \right)^*,

whose complex conjugates yield

.. math::

    \left( X_{N / 2 - k}^e \right)^*
    &
    =
    +
    \frac{1}{2} Z_k
    +
    \frac{1}{2} \left( Z_{N / 2 - k} \right)^*
    =
    X_k^e,

    \left( X_{N / 2 - k}^o \right)^*
    &
    =
    -
    \frac{I}{2} Z_k
    +
    \frac{I}{2} \left( Z_{N / 2 - k} \right)^*
    =
    X_k^o.

By applying these relations to

.. math::

    X_k
    =
    X_k^e
    +
    \rdfttwiddle{-}{k}{N}
    X_k^o

with :math:`\seq{k}{N / 2}`, we obtain

.. math::

    X_{N / 2 - k}
    =
    &
    X_{N / 2 - k}^e
    +
    \rdfttwiddle{-}{N / 2 - k}{N}
    X_{N / 2 - k}^o

    =
    &
    X_{N / 2 - k}^e
    +
    \exp \left( - \pi I \right)
    \rdfttwiddle{}{k}{N}
    X_{N / 2 - k}^o

    =
    &
    X_{N / 2 - k}^e
    -
    \rdfttwiddle{}{k}{N}
    X_{N / 2 - k}^o.

The complex conjugate yields

.. math::

    \left( X_{N / 2 - k} \right)^*
    =
    &
    \left( X_{N / 2 - k}^e \right)^*
    -
    \left[
        \rdfttwiddle{}{k}{N}
        X_{N / 2 - k}^o
    \right]^*

    =
    &
    \left( X_{N / 2 - k}^e \right)^*
    -
    \rdfttwiddle{-}{k}{N}
    \left( X_{N / 2 - k}^o \right)^*

    =
    &
    X_k^e
    -
    \rdfttwiddle{-}{k}{N}
    X_k^o.

As a consequence, :math:`X_k^e` and :math:`X_k^o` are obtained from :math:`X_k` following:

.. math::

    X_k^e
    =
    \frac{1}{2}
    X_k
    +
    \frac{1}{2}
    \left( X_{N / 2 - k} \right)^*,

    X_k^o
    \rdfttwiddle{-}{k}{N}
    =
    \frac{1}{2}
    X_k
    -
    \frac{1}{2}
    \left( X_{N / 2 - k} \right)^*.

The signal in physical space :math:`z_n` with :math:`\seq{n}{N / 2 - 1}` is obtained by the inverse Fourier transform with respect to

.. math::

    Z_k
    =
    X_k^e
    +
    I
    X_k^o

with :math:`\seq{k}{N / 2 - 1}`, followed by

.. math::

    &
    x_{2 n    } = \Re \left( z_n \right),

    &
    x_{2 n + 1} = \Im \left( z_n \right),

with :math:`\seq{n}{N / 2 - 1}`.

