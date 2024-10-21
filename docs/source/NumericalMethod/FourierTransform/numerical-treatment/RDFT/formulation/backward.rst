The backward transform is obtained essentially by backtracing the forward procedures.
Note that :math:`X_k` should satisfy the Hermitian symmetry:

.. math::

    X_{N - k}^*
    =
    X_k

as discussed in the forward transform.

To start, we recall the definition of :math:`X_k^e` and :math:`X_k^o`:

.. math::

    X_k^e
    &
    \equiv
    +
    \frac{1}{2} Z_k
    +
    \frac{1}{2} Z_{N / 2 - k}^*,

    X_k^o
    &
    \equiv
    -
    \frac{I}{2} Z_k
    +
    \frac{I}{2} Z_{N / 2 - k}^*.

By assigning :math:`k \leftarrow N / 2 - k`, we obtain

.. math::

    X_{N / 2 - k}^e
    &
    =
    +
    \frac{1}{2} Z_{N / 2 - k}
    +
    \frac{1}{2} Z_k^*,

    X_{N / 2 - k}^o
    &
    =
    -
    \frac{I}{2} Z_{N / 2 - k}
    +
    \frac{I}{2} Z_k^*,

whose complex conjugates yield

.. math::

    \left( X_{N / 2 - k}^e \right)^*
    &
    =
    +
    \frac{1}{2} Z_k
    +
    \frac{1}{2} Z_{N / 2 - k}^*
    =
    X_k^e,

    \left( X_{N / 2 - k}^o \right)^*
    &
    =
    -
    \frac{I}{2} Z_k
    +
    \frac{I}{2} Z_{N / 2 - k}^*
    =
    X_k^o.

By applying these relations to

.. math::

    X_k
    =
    X_k^e
    +
    \twiddle{- 2 \pi}{k}{N}
    X_k^o

with :math:`\seq{k}{N / 2}`, we obtain

.. math::

    X_{N / 2 - k}
    =
    &
    X_{N / 2 - k}^e
    +
    \twiddle{- 2 \pi}{N / 2 - k}{N}
    X_{N / 2 - k}^o

    =
    &
    X_{N / 2 - k}^e
    +
    \exp \left( - \pi I \right)
    \twiddle{2 \pi}{k}{N}
    X_{N / 2 - k}^o

    =
    &
    X_{N / 2 - k}^e
    -
    \twiddle{2 \pi}{k}{N}
    X_{N / 2 - k}^o.

The complex conjugate yields

.. math::

    X_{N / 2 - k}^*
    =
    &
    \left( X_{N / 2 - k}^e \right)^*
    -
    \left[
        \twiddle{2 \pi}{k}{N}
        X_{N / 2 - k}^o
    \right]^*

    =
    &
    \left( X_{N / 2 - k}^e \right)^*
    -
    \twiddle{- 2 \pi}{k}{N}
    \left( X_{N / 2 - k}^o \right)^*

    =
    &
    X_k^e
    -
    \twiddle{- 2 \pi}{k}{N}
    X_k^o.

As a consequence, :math:`X_k^e` and :math:`X_k^o` are obtained from :math:`X_k` following:

.. math::

    X_k^e
    =
    \frac{1}{2}
    X_k
    +
    \frac{1}{2}
    X_{N / 2 - k}^*,

    X_k^o
    \twiddle{- 2 \pi}{k}{N}
    =
    \frac{1}{2}
    X_k
    -
    \frac{1}{2}
    X_{N / 2 - k}^*.

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

    x_{2 n    } &= 2 \Re \left( z_n \right),

    x_{2 n + 1} &= 2 \Im \left( z_n \right),

with :math:`\seq{n}{N / 2 - 1}`.
Note that the presence of pre-factor :math:`2` is because the size of the inverse Fourier transform to convert :math:`Z_k` to :math:`z_n` is :math:`N / 2`.

