We consider the inverse transform:

.. math::

    x_n
    =
    \idft{n}{N}{X_0}{X_1}{X_{N - 1}}
    =
    \frac{1}{N}
    \sum_{k = 0}^{N - 1}
    X_k
    \twiddle{2 \pi}{n k}{N}

for :math:`\seq{n}{N - 1}`, where :math:`X_k` should satisfy the Hermitian symmetry:

.. math::

    X_{N - k}^*
    =
    X_k.

Also it should be :ref:`ordered in a specific manner <rdft_order_of_signals>`.

The inverse transform is obtained essentially by backtracing the forward procedures.
To start, we calculate :math:`X_k^e` and :math:`X_k^o` following

.. math::

    X_k^e
    &
    =
    \frac{1}{2}
    \dft{k}{N}{x_0}{x_1}{x_{N - 1}}
    +
    \frac{1}{2}
    \dft{k + N / 2}{N}{x_0}{x_1}{x_{N - 1}},

    X_k^o
    \twiddle{- 2 \pi}{k}{N}
    &
    =
    \frac{1}{2}
    \dft{k}{N}{x_0}{x_1}{x_{N - 1}}
    -
    \frac{1}{2}
    \dft{k + N / 2}{N}{x_0}{x_1}{x_{N - 1}},

for :math:`\seq{k}{N / 2 - 1}`.

Note that, since :math:`X_k` does not store the entire signal, we need to recover the series of complex numbers:

.. myliteralinclude:: /../../NumericalMethod/FourierTransform/RDFT/src/rdft.c
    :language: c
    :tag: compute 1/2 X_k

.. myliteralinclude:: /../../NumericalMethod/FourierTransform/RDFT/src/rdft.c
    :language: c
    :tag: compute 1/2 X_{k + N / 2}

They are combined to yield :math:`X_k^e` and :math:`X_k^o`:

.. myliteralinclude:: /../../NumericalMethod/FourierTransform/RDFT/src/rdft.c
    :language: c
    :tag: X_k^e = (+ 1/2 X_k + 1/2 X_{k + N / 2})

.. myliteralinclude:: /../../NumericalMethod/FourierTransform/RDFT/src/rdft.c
    :language: c
    :tag: X_k^o = (+ 1/2 X_k - 1/2 X_{k + N / 2}) * exp(arg)

Using these values, :math:`Z_k` is recovered following

.. math::

    Z_k
    =
    X_k^e
    +
    I
    X_k^o:

.. myliteralinclude:: /../../NumericalMethod/FourierTransform/RDFT/src/rdft.c
    :language: c
    :tag: Z_k = X_k^e + X_k^o * I

For the corner case :math:`k = 0`, we have

.. math::

    X_0^e
    &
    =
    \frac{1}{2}
    X_0
    +
    \frac{1}{2}
    X_{N / 2}^*,
    =
    \frac{1}{2}
    X_0
    +
    \frac{1}{2}
    X_{N / 2},

    X_0^o
    &
    =
    \frac{1}{2}
    X_0
    -
    \frac{1}{2}
    X_{N / 2}^*
    =
    \frac{1}{2}
    X_0
    -
    \frac{1}{2}
    X_{N / 2},

and due to :math:`\imag{X_0} = \imag{X_{N / 2}} = 0`, we obtain

.. math::

    Z_0
    =
    X_0^e
    +
    I
    X_0^o.

.. myliteralinclude:: /../../NumericalMethod/FourierTransform/RDFT/src/rdft.c
    :language: c
    :tag: create a complex signal, edge cases

An inverse transform with respect to :math:`Z_k` is performed to yield :math:`z_n`:

.. math::

    z_n
    =
    \idft{n}{N / 2}{Z_0}{Z_1}{Z_{N / 2 - 1}}:

.. myliteralinclude:: /../../NumericalMethod/FourierTransform/RDFT/src/rdft.c
    :language: c
    :tag: compute complex idft to find z_n

From the result, we obtain

.. math::

    x_{2 n    } &= \real{z_n},

    x_{2 n + 1} &= \imag{z_n},

for :math:`\seq{n}{N / 2 - 1}`.
Since the output signal is already ordered in a desired manner, no additional manipulation is necessary here.

The whole process is given below for completeness:

.. myliteralinclude:: /../../NumericalMethod/FourierTransform/RDFT/src/rdft.c
    :language: c
    :tag: compute backward transform

