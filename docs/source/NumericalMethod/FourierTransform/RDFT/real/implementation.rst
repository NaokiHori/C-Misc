##############
Implementation
##############

=================
Forward transform
=================

The discrete Fourier transform of a sequence composed of real numbers :math:`x_n`:

.. math::

    X_k
    =
    \rdft{N}{x}{0}{1}{N - 1}
    =
    \sum_{n = 0}^{N - 1}
    x_n
    \rdfttwiddle{-}{n k}{N}

with :math:`\seq{k}{N - 1}`, can be computed as follows.

#. Pre-processing

    Create a signal :math:`z_n` composed of :math:`N / 2` complex numbers:

    .. math::

        z_n = x_{2 n} + I x_{2 n + 1},

    where :math:`\seq{n}{N / 2 - 1}`.

#. Transform

    Evaluate the discrete Fourier transform of :math:`z_n`:

    .. math::

        Z_k
        \equiv
        \rdft{N / 2}{z}{0}{1}{N / 2 - 1}

    with :math:`\seq{k}{N / 2 - 1}`.

#. Post-processing

    Using :math:`Z_k`, compute

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

    where :math:`\seq{k}{N / 2}`.

    Note that, to evaluate :math:`k = 0`, we need to use

    .. math::

        \left( Z_{N / 2} \right)^*
        =
        \left( Z_0 \right)^*,

    giving

    .. math::

        X_0^e
        &
        =
        +
        \frac{1}{2}
        Z_0
        +
        \frac{1}{2}
        \left( Z_0 \right)^*,

        X_0^o
        &
        =
        -
        \frac{I}{2}
        Z_0
        +
        \frac{I}{2}
        \left( Z_0 \right)^*.

    Similarly, for :math:`k = N / 2`, we have

    .. math::

        X_{N / 2}^e
        &
        =
        +
        \frac{1}{2}
        Z_0
        +
        \frac{1}{2}
        \left( Z_0 \right)^*,

        X_{N / 2}^o
        &
        =
        -
        \frac{I}{2}
        Z_0
        +
        \frac{I}{2}
        \left( Z_0 \right)^*.

    The final result is obtained by combining :math:`X_k^e` and :math:`X_k^o`:

    .. math::

        X_k
        =
        X_k^e
        +
        \rdfttwiddle{-}{k}{N}
        X_k^o,

    where :math:`\seq{k}{N / 2}`.

==================
Backward transform
==================

The inverse discrete Fourier transform of a sequence composed of complex numbers :math:`X_k`:

.. math::

    x_n
    =
    \irdft{N}{X}{0}{1}{N - 1}
    \equiv
    \sum_{k = 0}^{N - 1}
    X_k
    \rdfttwiddle{+}{n k}{N}

with :math:`\seq{n}{N - 1}`, can be computed as follows.
Note that :math:`X_k` should satisfy

.. math::

    X_{N - k}
    =
    \left( X_k \right)^*,

and thus only :math:`k` up to :math:`N / 2` are stored in practice.
Also this implies that :math:`\Im \left( X_0 \right) = \Im \left( X_{N / 2} \right) = 0`.

#. Pre-processing

    Using the input signal, compute

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
        \left( X_{N / 2 - k} \right)^*,

    from which

    .. math::

        Z_k
        =
        X_k^e
        +
        I
        X_k^o

    is obtained, with :math:`\seq{k}{N / 2 - 1}`.

#. Transform

    Evaluate the inverse discrete Fourier transform of :math:`X_k`:

    .. math::

        z_n
        \equiv
        \irdft{N / 2}{Z}{0}{1}{N / 2 - 1}

    with :math:`\seq{n}{N / 2 - 1}`.

#. Post-processing

    From :math:`z_n`, obtain the signal in physical space following:

    .. math::

        &
        x_{2 n    } = \Re \left( z_n \right),

        &
        x_{2 n + 1} = \Im \left( z_n \right),

    with :math:`\seq{n}{N / 2 - 1}`.

