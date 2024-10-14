The inverse discrete Fourier transform of a sequence composed of complex numbers :math:`X_k`:

.. math::

    x_n
    =
    \irdft{N}{X}{0}{1}{N - 1}
    \equiv
    \sum_{k = 0}^{N - 1}
    X_k
    \rdfttwiddle{}{n k}{N}

with :math:`\seq{n}{N - 1}`, can be computed as follows.
Note that :math:`X_k` should satisfy the Hermitian symmetry:

.. math::

    X_{N - k}
    =
    X_k^*,

and thus :math:`X_k` are stored only up to :math:`k = N / 2` in practice.

#. Pre-processing

    Using the input signal, compute

    .. math::

        X_k^e
        =
        \frac{1}{2}
        X_k
        +
        \frac{1}{2}
        X_{N / 2 - k}^*,

        X_k^o
        =
        \frac{I}{2}
        X_k
        -
        \frac{I}{2}
        X_{N / 2 - k}^*,

    from which a complex signal

    .. math::

        Z_k
        =
        X_k^e
        +
        \rdfttwiddle{}{k}{N}
        X_k^o

    is constructed, with :math:`\seq{k}{N / 2 - 1}`.

    .. myliteralinclude:: /../../NumericalMethod/FourierTransform/RDFT/src/rdft.c
        :language: c
        :tag: create a complex signal, common cases

    For :math:`k = 0`, we have

    .. math::

        X_0^e
        =
        \frac{1}{2}
        X_0
        +
        \frac{1}{2}
        X_{N / 2}^*,

        X_0^o
        =
        \frac{I}{2}
        X_0
        -
        \frac{I}{2}
        X_{N / 2}^*.

    Recall that :math:`\Im \left( X_0 \right) = \Im \left( X_{N / 2} \right) = 0`, and thus

    .. math::

        Z_0
        =
        X_0^e
        +
        X_0^o.

    .. myliteralinclude:: /../../NumericalMethod/FourierTransform/RDFT/src/rdft.c
        :language: c
        :tag: create a complex signal, edge cases

#. Transform

    Evaluate the inverse discrete Fourier transform of :math:`X_k`:

    .. math::

        z_n
        \equiv
        \irdft{N / 2}{Z}{0}{1}{N / 2 - 1}

    with :math:`\seq{n}{N / 2 - 1}`.

    .. myliteralinclude:: /../../NumericalMethod/FourierTransform/RDFT/src/rdft.c
        :language: c
        :tag: from spectral to physical space

#. Post-processing

    From :math:`z_n`, obtain the signal in physical space following:

    .. math::

        x_{2 n    } & = \Re \left( z_n \right),

        x_{2 n + 1} & = \Im \left( z_n \right),

    with :math:`\seq{n}{N / 2 - 1}`.
    The output signal is ordered as this way, and thus no additional procedure is necessary here.
