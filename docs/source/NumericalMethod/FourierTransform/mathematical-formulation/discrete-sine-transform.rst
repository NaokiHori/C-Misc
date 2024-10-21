
.. _discrete_sine_transform:

#######################
Discrete sine transform
#######################

As we did to derive :ref:`the discrete cosine transform <discrete_cosine_transform>`, we consider a shifted discrete Fourier transform of :math:`2 N` real numbers.
Here, instead of the even symmetry, we consider a signal satisfying the odd symmetry:

.. math::

    x_{2 N - 1 - n}
    =
    -
    x_n

with :math:`\seq{n}{2 N - 1}`.
Due to

.. math::

    \sum_{n = N}^{2 N - 1}
    x_n
    \twiddle{- 2 \pi}{\left( n + \frac{1}{2} \right) k}{2 N}
    =
    -
    \sum_{n = 0}^{N - 1}
    x_n
    \twiddle{2 \pi}{\left( n + \frac{1}{2} \right) k}{2 N},

.. derivation_disclosure:: 

    With :math:`m = 2 N - 1 - n`, we have

    .. math::

        \sum_{m = N - 1}^{0}
        x_{2 N - 1 - m}
        \twiddle{- 2 \pi}{\left( 2 N - 1 - m + \frac{1}{2} \right) k}{2 N}
        &
        =
        -
        \sum_{n = 0}^{N - 1}
        x_n
        \exp \left( - 2 \pi k I \right)
        \twiddle{2 \pi}{\left( n + \frac{1}{2} \right) k}{2 N}

        &
        =
        -
        \sum_{n = 0}^{N - 1}
        x_n
        \twiddle{2 \pi}{\left( n + \frac{1}{2} \right) k}{2 N}.

we obtain

.. math::

    X_k
    =
    -
    2
    I
    \sum_{n = 0}^{N - 1}
    x_n
    \stwiddle{2 \pi}{\left( n + \frac{1}{2} \right) k}{2 N}.

It is clear that :math:`X_k \in \mathbb{I}`.
Note that

.. math::

    X_{2 N - k}
    =
    X_k,

.. derivation_disclosure:: 

    .. math::

        X_{2 N - k}
        &
        =
        -
        2
        I
        \sum_{n = 0}^{N - 1}
        x_n
        \stwiddle{2 \pi}{\left( n + \frac{1}{2} \right) \left( 2 N - k \right)}{2 N}

        &
        =
        -
        2
        I
        \sum_{n = 0}^{N - 1}
        x_n
        \sin
        \left(
            \pi \left( 2 n + 1 \right)
            -
            2 \pi \frac{\left( n + \frac{1}{2} \right) k}{2 N}
        \right)

        &
        =
        -
        2
        I
        \sum_{n = 0}^{N - 1}
        x_n
        \stwiddle{2 \pi}{\left( n + \frac{1}{2} \right) k}{2 N}
        \,\,
        \left(
            \because \cos \left\{ \pi \left( 2 n + 1 \right) \right\} = -1
        \right)

        &
        =
        X_k.

and thus it is sufficient to consider :math:`\seq{k}{N}`.
Additionally we easily see that :math:`X_0 \equiv 0`.

The inverse transform can be found as follows; due to

.. math::

    \sum_{k = N}^{2 N - 1}
    X_k
    \twiddle{2 \pi}{\left( n + \frac{1}{2} \right) k}{2 N}
    =
    -
    \sum_{k = 1}^{N - 1}
    X_k
    \twiddle{- 2 \pi}{\left( n + \frac{1}{2} \right) k}{2 N}
    +
    \left( -1 \right)^n
    I
    X_N,

.. derivation_disclosure:: 

    .. math::

        \sum_{k = N}^{2 N - 1}
        X_k
        \twiddle{2 \pi}{\left( n + \frac{1}{2} \right) k}{2 N}
        &
        =
        \sum_{k = N}^{2 N - 1}
        X_{2 N - k}
        \twiddle{2 \pi}{\left( n + \frac{1}{2} \right) k}{2 N}

        &
        =
        \sum_{l = N}^{1}
        X_l
        \twiddle{2 \pi}{\left( n + \frac{1}{2} \right) \left( 2 N - l \right)}{2 N}
        \,\,
        \left( l \equiv 2 N - k \right)

        &
        =
        \sum_{k = 1}^{N}
        X_k
        \exp \left\{ \pi \left( 2 n + 1 \right) I \right\}
        \twiddle{- 2 \pi}{\left( n + \frac{1}{2} \right) k}{2 N}

        &
        =
        -
        \sum_{k = 1}^{N}
        X_k
        \twiddle{- 2 \pi}{\left( n + \frac{1}{2} \right) k}{2 N}
        \,\,
        \left( \because \exp \left\{ \pi \left( 2 n + 1 \right) I \right\} = -1 \right)

        &
        =
        -
        \sum_{k = 1}^{N - 1}
        X_k
        \twiddle{- 2 \pi}{\left( n + \frac{1}{2} \right) k}{2 N}
        +
        \left( -1 \right)^n
        I
        X_N.

we obtain

.. math::

    x_n
    &
    =
    \frac{1}{2 N}
    \left\{
        \sum_{k = 0}^{N - 1}
        X_k
        \twiddle{2 \pi}{\left( n + \frac{1}{2} \right) k}{2 N}
        -
        \sum_{k = 1}^{N - 1}
        X_k
        \twiddle{- 2 \pi}{\left( n + \frac{1}{2} \right) k}{2 N}
        +
        \left( -1 \right)^n
        I
        X_N
    \right\}

    &
    =
    \frac{I}{2 N}
    \left\{
        2
        \sum_{k = 1}^{N - 1}
        X_k
        \stwiddle{2 \pi}{\left( n + \frac{1}{2} \right) k}{2 N}
        +
        \left( -1 \right)^n
        X_N
    \right\}.

To summarize, the forward and the inverse transforms lead to

.. math::

    X_k
    =
    -
    2
    I
    \sum_{n = 0}^{N - 1}
    x_n
    \stwiddle{2 \pi}{\left( n + \frac{1}{2} \right) k}{2 N}

and

.. math::

    x_n
    =
    \frac{I}{2 N}
    \left\{
        2
        \sum_{k = 1}^{N - 1}
        X_k
        \stwiddle{2 \pi}{\left( n + \frac{1}{2} \right) k}{2 N}
        +
        \left( -1 \right)^n
        X_N
    \right\},

respectively.

By assigning :math:`X_k \leftarrow - I X_k`, we find the discrete sine transform of type II and type III as

.. math::

    X_k
    =
    2
    \sum_{n = 0}^{N - 1}
    x_n
    \stwiddle{2 \pi}{\left( n + \frac{1}{2} \right) k}{2 N}

with :math:`\seq{k}{N}`, and

.. math::

    x_n
    =
    \frac{1}{2 N}
    \left\{
        2
        \sum_{k = 1}^{N - 1}
        X_k
        \stwiddle{2 \pi}{\left( n + \frac{1}{2} \right) k}{2 N}
        +
        \left( -1 \right)^n
        X_N
    \right\}

with :math:`\seq{n}{N - 1}`, respectively.
Note that both transforms are :math:`\mathbb{R}^N \rightarrow \mathbb{R}^N` since :math:`X_0 = 0`.

