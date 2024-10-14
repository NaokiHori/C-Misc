The discrete Fourier transform of a sequence composed of real numbers :math:`x_n`:

.. math::

    X_k
    \equiv
    \rdft{N}{x}{0}{1}{N - 1}
    \equiv
    \sum_{n = 0}^{N - 1}
    x_n
    \rdfttwiddle{-}{n k}{N}

with :math:`\seq{k}{N / 2}` due to the Hermitian symmetry, can be computed as follows.

#. Pre-processing

    Create a signal :math:`z_n` composed of :math:`N / 2` complex numbers:

    .. math::

        z_n = x_{2 n} + I x_{2 n + 1},

    where :math:`\seq{n}{N / 2 - 1}`.
    Since we request the input signal to be ordered as this way, no additional procedure is necessary here.

#. Transform

    Evaluate the discrete Fourier transform of a complex sequence composed of :math:`N / 2` numbers :math:`z_n`:

    .. math::

        Z_k
        \equiv
        \rdft{N / 2}{z}{0}{1}{N / 2 - 1}

    with :math:`\seq{k}{N / 2 - 1}`.

    .. myliteralinclude:: /../../NumericalMethod/FourierTransform/RDFT/src/rdft.c
        :language: c
        :tag: from physical to spectral space

#. Post-processing

    Using :math:`Z_k`, compute

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

    where :math:`\seq{k}{N / 2}`.
    By combining :math:`X_k^e` and :math:`X_k^o`, calculate

    .. math::

        X_k
        =
        X_k^e
        +
        \rdfttwiddle{-}{k}{N}
        X_k^o,

    with :math:`\seq{k}{N / 2}`.

    .. myliteralinclude:: /../../NumericalMethod/FourierTransform/RDFT/src/rdft.c
        :language: c
        :tag: compute FFT of the original real signal, common cases

    For the edge cases :math:`k = 0, N / 2`, due to :math:`Z_{N / 2} = Z_0`, we have

    .. math::

        X_0^e
        =
        X_{N / 2}^e
        &
        =
        +
        \frac{1}{2}
        Z_0
        +
        \frac{1}{2}
        Z_0^*
        =
        \Re \left( Z_0 \right)
        \in
        \mathbb{R},

        X_0^o
        =
        X_{N / 2}^o
        &
        =
        -
        \frac{I}{2}
        Z_0
        +
        \frac{I}{2}
        Z_0^*
        =
        \Im \left( Z_0 \right)
        \in
        \mathbb{R},

    and as a result

    .. math::

        X_0
        &
        =
        X_0^e
        +
        X_0^o
        \in
        \mathbb{R},

        X_{N / 2}
        &
        =
        X_0^e
        -
        X_0^o
        \in
        \mathbb{R}.

    .. myliteralinclude:: /../../NumericalMethod/FourierTransform/RDFT/src/rdft.c
        :language: c
        :tag: compute FFT of the original real signal, edge cases

