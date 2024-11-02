We consider the forward transform:

.. math::

    X_k
    =
    \dft{k}{N}{x_0}{x_1}{x_{N - 1}}
    =
    \sum_{n = 0}^{N - 1}
    x_n
    \twiddle{- 2 \pi}{n k}{N}

for :math:`\seq{k}{0}{1}{N - 1}`.
Although this can be computed as the discrete Fourier transform of a complex sequence having zero imaginary numbers, we aim at computing it with less computational effort here.

By utilizing the decimation in time (assuming :math:`N` is a multiple of :math:`2`), we have

.. math::

    \dft{k}{N}{x_0}{x_1}{x_{N - 1}}
    &
    =
    \dft{k}{N / 2}{x_0}{x_2}{x_{N - 2}}
    +
    \twiddle{- 2 \pi}{k}{N}
    \dft{k}{N / 2}{x_1}{x_3}{x_{N - 1}},

    \dft{k + N / 2}{N}{x_0}{x_1}{x_{N - 1}}
    &
    =
    \dft{k}{N / 2}{x_0}{x_2}{x_{N - 2}}
    -
    \twiddle{- 2 \pi}{k}{N}
    \dft{k}{N / 2}{x_1}{x_3}{x_{N - 1}},

for :math:`\seq{k}{0}{1}{N / 2 - 1}`.
Instead of taking this recurrence relation as it is, we introduce :math:`z_n` defined as

.. math::

    z_n
    =
    x_{2 n}
    +
    I
    x_{2 n + 1}

for :math:`\seq{n}{0}{1}{N / 2 - 1}`, giving

.. math::

    x_{2 n    } &= + \frac{1}{2} z_n + \frac{1}{2} z_n^*,

    x_{2 n + 1} &= - \frac{I}{2} z_n + \frac{I}{2} z_n^*.

Their discrete Fourier transforms, due to the linearity, lead to

.. math::

    \dft{k}{N / 2}{x_0}{x_2}{x_{N - 2}}
    &
    =
    +
    \frac{1}{2}
    \dft{k}{N / 2}{z_0}{z_1}{z_{N / 2 - 1}}
    +
    \frac{1}{2}
    \dft{k}{N / 2}{z_0^*}{z_1^*}{z_{N / 2 - 1}^*},

    \dft{k}{N / 2}{x_1}{x_3}{x_{N - 1}}
    &
    =
    -
    \frac{I}{2}
    \dft{k}{N / 2}{z_0}{z_1}{z_{N / 2 - 1}}
    +
    \frac{I}{2}
    \dft{k}{N / 2}{z_0^*}{z_1^*}{z_{N / 2 - 1}^*},

for :math:`\seq{k}{0}{1}{N / 2 - 1}`.

Now two discrete Fourier transforms are involved, and we focus on the latter:

.. math::

    \dft{k}{N / 2}{z_0^*}{z_1^*}{z_{N / 2 - 1}^*}
    =
    \sum_{n = 0}^{N / 2 - 1}
    z_n^*
    \twiddle{- 2 \pi}{n k}{N / 2}.

Since this is equal to

.. math::

    \left(
        \dft{N / 2 - k}{N / 2}{z_0}{z_1}{z_{N / 2 - 1}}
    \right)^*,

.. derivation_disclosure::

    .. math::

        \dft{k}{N / 2}{z_0^*}{z_1^*}{z_{N / 2 - 1}^*}
        =
        &
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
        \left(
            \dft{N / 2 - k}{N / 2}{z_0}{z_1}{z_{N / 2 - 1}}
        \right)^*.

we notice that the latter relation can be obtained from the former; namely we need to evaluate only one discrete Fourier transform

.. math::

    \dft{N / 2 - k}{N / 2}{z_0}{z_1}{z_{N / 2 - 1}}

to find

.. math::

    \dft{k}{N / 2}{x_0}{x_2}{x_{N - 2}}
    &
    =
    +
    \frac{1}{2}
    \dft{k}{N / 2}{z_0}{z_1}{z_{N / 2 - 1}}
    +
    \frac{1}{2}
    \left( \dft{N / 2 - k}{N / 2}{z_0}{z_1}{z_{N / 2 - 1}} \right)^*,

    \dft{k}{N / 2}{x_1}{x_3}{x_{N - 1}}
    &
    =
    -
    \frac{I}{2}
    \dft{k}{N / 2}{z_0}{z_1}{z_{N / 2 - 1}}
    +
    \frac{I}{2}
    \left( \dft{N / 2 - k}{N / 2}{z_0}{z_1}{z_{N / 2 - 1}} \right)^*,

for :math:`\seq{k}{0}{1}{N / 2 - 1}`.

For the corner case :math:`k = 0`, we see

.. math::

    \dft{k = N / 2}{N / 2}{z_0}{z_1}{z_{N / 2 - 1}}
    &
    =
    \sum_{n = 0}^{N / 2 - 1}
    z_n
    \twiddle{- 2 \pi}{n N / 2}{N / 2}

    &
    =
    \sum_{n = 0}^{N / 2 - 1}
    z_n

    &
    =
    \dft{k = 0}{N / 2}{z_0}{z_1}{z_{N / 2 - 1}}.

In summary, the discrete Fourier transform of a real sequence is computed as follows.

First, we create a signal :math:`z_n` composed of :math:`N / 2` complex numbers:

.. math::

    z_n
    =
    x_{2 n}
    +
    I x_{2 n + 1},

where :math:`\seq{n}{0}{1}{N / 2 - 1}`.
Practically, this can be achieved merely by casting the user input as a series of complex numbers; namely no data manipulations are needed.

Next, the discrete Fourier transform of :math:`z_n` is evaluated to find

.. math::

    Z_k
    \equiv
    \dft{k}{N / 2}{z_0}{z_1}{z_{N / 2 - 1}}

for :math:`\seq{k}{0}{1}{N / 2 - 1}`:

.. myliteralinclude:: /../../NumericalMethod/FourierTransform/RDFT/src/rdft.c
    :language: c
    :tag: compute complex dft to find Z_k

.. myliteralinclude:: /../../NumericalMethod/FourierTransform/RDFT/src/rdft.c
    :language: c
    :tag: compute 1/2 Z_k

.. myliteralinclude:: /../../NumericalMethod/FourierTransform/RDFT/src/rdft.c
    :language: c
    :tag: compute 1/2 Z_{N / 2 - k}

The result is used to compute

.. math::

    X_k^e
    &
    \equiv
    \dft{k}{N / 2}{x_0}{x_2}{x_{N - 2}}
    =
    +
    \frac{1}{2}
    Z_k
    +
    \frac{1}{2}
    Z_{N / 2 - k}^*,

    X_k^o
    &
    \equiv
    \dft{k}{N / 2}{x_1}{x_3}{x_{N - 1}}
    =
    -
    \frac{I}{2}
    Z_k
    +
    \frac{I}{2}
    Z_{N / 2 - k}^*,

where :math:`\seq{k}{0}{1}{N / 2 - 1}`:

.. myliteralinclude:: /../../NumericalMethod/FourierTransform/RDFT/src/rdft.c
    :language: c
    :tag: compute X_k^e = + 1/2 Z_k + 1/2 Z_{N / 2 - k}^*

.. myliteralinclude:: /../../NumericalMethod/FourierTransform/RDFT/src/rdft.c
    :language: c
    :tag: compute X_k^o = - I/2 Z_k + I/2 Z_{N / 2 - k}^*

Finally they are adopted to calculate

.. math::

    \dft{k}{N}{x_0}{x_1}{x_{N - 1}}
    &
    =
    X_k^e
    +
    \twiddle{- 2 \pi}{k}{N}
    X_k^o,

    \dft{k + N / 2}{N}{x_0}{x_1}{x_{N - 1}}
    &
    =
    X_k^e
    -
    \twiddle{- 2 \pi}{k}{N}
    X_k^o,

for :math:`\seq{k}{0}{1}{N / 2 - 1}`.

Due to :ref:`the order of signals <rdft_order_of_signals>`, it is worthwhile to decompose the real and the imaginary parts:

.. math::

    \real{\dft{k}{N}{x_0}{x_1}{x_{N - 1}}}
    &
    =
    \real{X_k^e}
    +
    \left\{
        +
        \real{X_k^o}
        \ctwiddle{- 2 \pi}{k}{N}
        -
        \imag{X_k^o}
        \stwiddle{- 2 \pi}{k}{N}
    \right\},

    \imag{\dft{k + N / 2}{N}{x_0}{x_1}{x_{N - 1}}}
    &
    =
    \imag{X_k^e}
    +
    \left\{
        -
        \real{X_k^o}
        \stwiddle{- 2 \pi}{k}{N}
        -
        \imag{X_k^o}
        \ctwiddle{- 2 \pi}{k}{N}
    \right\}:

.. myliteralinclude:: /../../NumericalMethod/FourierTransform/RDFT/src/rdft.c
    :language: c
    :tag: real and imaginary parts are stored separately

Note that

.. math::

    Z_{N / 2}
    =
    Z_0

is used to get

.. math::

    X_0^e
    &
    =
    \real{Z_0},

    X_0^o
    &
    =
    \imag{Z_0},

and as a consequence:

.. math::

    \dft{0}{N}{x_0}{x_1}{x_{N - 1}}
    &
    =
    X_0^e
    +
    X_0^o,

    \dft{N / 2}{N}{x_0}{x_1}{x_{N - 1}}
    &
    =
    X_0^e
    -
    X_0^o:

.. myliteralinclude:: /../../NumericalMethod/FourierTransform/RDFT/src/rdft.c
    :language: c
    :tag: compute FFT of the original real signal, corner cases

The whole process is given below for completeness:

.. myliteralinclude:: /../../NumericalMethod/FourierTransform/RDFT/src/rdft.c
    :language: c
    :tag: compute forward transform

