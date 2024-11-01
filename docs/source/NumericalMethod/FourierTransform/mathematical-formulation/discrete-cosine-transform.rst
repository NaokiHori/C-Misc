
.. _discrete_cosine_transform:

#########################
Discrete cosine transform
#########################

We consider :ref:`a shifted discrete Fourier transform <shifted_discrete_fourier_transform>` of :math:`2 N` real numbers:

.. math::

    X_k
    &
    =
    \sum_{n = 0}^{2 N - 1}
    x_n
    \twiddle{- 2 \pi}{\left( n + \frac{1}{2} \right) k}{2 N}

    &
    =
    \sum_{n = 0}^{N - 1}
    x_n
    \twiddle{- 2 \pi}{\left( n + \frac{1}{2} \right) k}{2 N}
    +
    \sum_{n = N}^{2 N - 1}
    x_n
    \twiddle{- 2 \pi}{\left( n + \frac{1}{2} \right) k}{2 N}

with :math:`\seq{k}{2 N - 1}`, and the corresponding inverse transform:

.. math::

    x_n
    &
    =
    \frac{1}{2 N}
    \sum_{k = 0}^{2 N - 1}
    X_k
    \twiddle{2 \pi}{\left( n + \frac{1}{2} \right) k}{2 N}

    &
    =
    \frac{1}{2 N}
    \sum_{k = 0}^{N - 1}
    X_k
    \twiddle{2 \pi}{\left( n + \frac{1}{2} \right) k}{2 N}
    +
    \frac{1}{2 N}
    \sum_{k = N}^{2 N - 1}
    X_k
    \twiddle{2 \pi}{\left( n + \frac{1}{2} \right) k}{2 N}

with :math:`\seq{n}{2 N - 1}`.
Although :math:`x_n` can be arbitrary, here we request the signal to satisfy

.. math::

    x_{2 N - 1 - n}
    =
    x_n

with :math:`\seq{n}{2 N - 1}`.
Due to

.. math::

    \sum_{n = N}^{2 N - 1}
    x_n
    \twiddle{- 2 \pi}{\left( n + \frac{1}{2} \right) k}{2 N}
    =
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
        \sum_{n = 0}^{N - 1}
        x_n
        \exp \left( - 2 \pi k I \right)
        \twiddle{2 \pi}{\left( n + \frac{1}{2} \right) k}{2 N}

        &
        =
        \sum_{n = 0}^{N - 1}
        x_n
        \twiddle{2 \pi}{\left( n + \frac{1}{2} \right) k}{2 N}.

we obtain

.. math::

    X_k
    &
    =
    \sum_{n = 0}^{N - 1}
    x_n
    \twiddle{- 2 \pi}{\left( n + \frac{1}{2} \right) k}{2 N}
    +
    \sum_{n = 0}^{N - 1}
    x_n
    \twiddle{2 \pi}{\left( n + \frac{1}{2} \right) k}{2 N}

    &
    =
    2
    \sum_{n = 0}^{N - 1}
    x_n
    \ctwiddle{2 \pi}{\left( n + \frac{1}{2} \right) k}{2 N}.

It is readily apparent that :math:`X_k \in \mathbb{R}`.
Note that

.. math::

    X_{2 N - k}
    =
    -
    X_k,

.. derivation_disclosure::

    .. math::

        X_{2 N - k}
        &
        =
        2
        \sum_{n = 0}^{N - 1}
        x_n
        \ctwiddle{2 \pi}{\left( n + \frac{1}{2} \right) \left( 2 N - k \right)}{2 N}

        &
        =
        2
        \sum_{n = 0}^{N - 1}
        x_n
        \cos
        \left(
            \pi \left( 2 n + 1 \right)
            -
            2 \pi \frac{\left( n + \frac{1}{2} \right) k}{2 N}
        \right)

        &
        =
        -
        2
        \sum_{n = 0}^{N - 1}
        x_n
        \ctwiddle{2 \pi}{\left( n + \frac{1}{2} \right) k}{2 N}

        &
        =
        -
        X_k.

and thus it is sufficient to consider :math:`\seq{k}{N - 1}`.
Also by utilizing this relation, we find

.. math::

    \sum_{k = N}^{2 N - 1}
    X_k
    \twiddle{2 \pi}{\left( n + \frac{1}{2} \right) k}{2 N}
    =
    \sum_{k = 1}^{N - 1}
    X_k
    \twiddle{- 2 \pi}{\left( n + \frac{1}{2} \right) k}{2 N},

.. derivation_disclosure::

    Assigning :math:`k \leftarrow 2 N - k` to the relation of the discrete cosine transform (type II) yields

    .. math::

        \sum_{k = N}^{2 N - 1}
        X_k
        \twiddle{2 \pi}{\left( n + \frac{1}{2} \right) k}{2 N}
        &
        =
        -
        \sum_{k = N}^{2 N - 1}
        X_{2 N - k}
        \twiddle{2 \pi}{\left( n + \frac{1}{2} \right) k}{2 N}

        &
        =
        -
        \sum_{l = N}^{1}
        X_l
        \twiddle{2 \pi}{\left( n + \frac{1}{2} \right) \left( 2 N - l \right)}{2 N}
        \,\,
        \left( l \equiv 2 N - k \right)

        &
        =
        -
        \sum_{k = 1}^{N}
        X_k
        \exp \left\{ \pi \left( 2 n + 1 \right) I \right\}
        \twiddle{- 2 \pi}{\left( n + \frac{1}{2} \right) k}{2 N}

        &
        =
        \sum_{k = 1}^{N}
        X_k
        \twiddle{- 2 \pi}{\left( n + \frac{1}{2} \right) k}{2 N}
        \,\,
        \left( \because \exp \left\{ \pi \left( 2 n + 1 \right) I \right\} = -1 \right)

        &
        =
        \sum_{k = 1}^{N - 1}
        X_k
        \twiddle{- 2 \pi}{\left( n + \frac{1}{2} \right) k}{2 N}
        \,\,
        \left( \because X_N = 0 \right).

from which the inverse transform leads to

.. math::

    x_n
    &
    =
    \frac{1}{2 N}
    \sum_{k = 0}^{N - 1}
    X_k
    \twiddle{2 \pi}{\left( n + \frac{1}{2} \right) k}{2 N}
    +
    \frac{1}{2 N}
    \sum_{k = 1}^{N - 1}
    X_k
    \twiddle{- 2 \pi}{\left( n + \frac{1}{2} \right) k}{2 N}

    &
    =
    \frac{1}{2 N}
    \left\{
        X_0
        +
        2
        \sum_{k = 1}^{N - 1}
        X_k
        \ctwiddle{2 \pi}{\left( n + \frac{1}{2} \right) k}{2 N}
    \right\}.

To summarize, the discrete cosine transform of type II and type III are defined as

.. math::

    X_k
    \equiv
    2
    \sum_{n = 0}^{N - 1}
    x_n
    \ctwiddle{2 \pi}{\left( n + \frac{1}{2} \right) k}{2 N}

with :math:`\seq{k}{N - 1}`, and

.. math::

    x_n
    \equiv
    \frac{1}{2 N}
    \left\{
        X_0
        +
        2
        \sum_{k = 1}^{N - 1}
        X_k
        \ctwiddle{2 \pi}{\left( n + \frac{1}{2} \right) k}{2 N}
    \right\}

with :math:`\seq{n}{N - 1}`, respectively.
Note that both transforms are :math:`\mathbb{R}^N \rightarrow \mathbb{R}^N`.

