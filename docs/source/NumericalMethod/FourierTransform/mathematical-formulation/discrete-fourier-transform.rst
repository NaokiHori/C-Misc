##########################
Discrete Fourier transform
##########################

We consider the discrete counterpart of the relation discussed in :ref:`the previous part <mathematical_formulation_fourier_series>`:

.. math::

    F_k
    =
    \frac{1}{L}
    \int_{0}^{L}
    f \left( x \right)
    \twiddle{- 2 \pi}{k x}{L}
    dx.

In particular, we describe the continuous function :math:`f \left( x \right)` as the superposition of :math:`N` Dirac delta functions having non-zero value at :math:`x_n \in \left[ 0, L \right)`, yielding

.. math::

    F_k
    &
    \approx
    \frac{1}{L}
    \int_{0}^{L}
    \sum_{n = 0}^{N - 1}
    f_n
    \delta
    \left(
        x
        -
        x_n
    \right)
    \twiddle{- 2 \pi}{k x}{L}
    dx

    &
    =
    \sum_{n = 0}^{N - 1}
    f_n
    \twiddle{- 2 \pi}{k x_n}{L}.

It should be noted that, the integral and the summation are interchanged, which needs careful consideration but is accepted anyway here.

Although the sampling points :math:`x_n` can be arbitrary chosen, we let them

.. math::

    x_n
    =
    n
    \frac{
        L
    }{
        N
    },

leading to

.. math::

    F_k
    =
    \sum_{n = 0}^{N - 1}
    f_n
    \twiddle{- 2 \pi}{k n}{N}.

Correspondingly, we notice

.. math::

    \frac{1}{N}
    \sum_{k = 0}^{N - 1}
    F_k
    \twiddle{2 \pi}{k n}{N}
    &
    =
    \sum_{k = 0}^{N - 1}
    \left\{
        \sum_{m = 0}^{N - 1}
        f_m
        \twiddle{- 2 \pi}{k m}{N}
    \right\}
    \twiddle{2 \pi}{k n}{N}

    &
    =
    \frac{1}{N}
    \sum_{m = 0}^{N - 1}
    \left\{
        f_m
        \sum_{k = 0}^{N - 1}
        \twiddle{2 \pi}{k \left( n - m \right)}{N}
    \right\}

    &
    =
    \frac{1}{N}
    \sum_{m = 0}^{N - 1}
    f_m
    N
    \delta_{nm}

    &
    =
    f_n,

where we use an identity with :math:`n \in \mathbb{Z}`:

.. math::

    \sum_{k = 0}^{N - 1}
    \twiddle{2 \pi}{k n}{N}
    =
    N
    \delta_{n0}.

.. derivation_disclosure:: 

    For :math:`n = 0` (or in general :math:`n = N m` with :math:`m \in \mathbb{Z}`), it is obvious that

    .. math::

        \sum_{k = 0}^{N - 1}
        1
        =
        N.

    Otherwise, we have

    .. math::

        \sum_{k = 0}^{N - 1}
        \twiddle{2 \pi}{n k}{N}
        =
        \frac{
            1 - \exp \left( 2 \pi n I \right)
        }{
            1 - \twiddle{2 \pi}{n}{N}
        }
        =
        0,

    where we adopt the formula to calculate the sum of geometric progression.

In summary, the discrete Fourier transform and its inverse transform are given by:

.. math::

    F_k
    &
    \equiv
    \sum_{n = 0}^{N - 1}
    f_n
    \twiddle{- 2 \pi}{k n}{N}
    \equiv
    \dft{k}{N}{f_0}{f_1}{f_{N - 1}},

    f_n
    &
    \equiv
    \frac{1}{N}
    \sum_{k = 0}^{N - 1}
    F_k
    \twiddle{2 \pi}{k n}{N}
    \equiv
    \idft{n}{N}{F_0}{F_1}{F_{N - 1}},

where :math:`\seq{k}{N - 1}` and :math:`\seq{n}{N - 1}`, respectively.
Note that both transforms are linear operators yielding linear maps: :math:`\mathbb{C}^N \rightarrow \mathbb{C}^N`.

