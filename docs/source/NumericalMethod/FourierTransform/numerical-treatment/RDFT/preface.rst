We consider the discrete Fourier transform of a sequence composed of :math:`N` real numbers :math:`x_n`:

.. math::

    X_k
    =
    \dft{k}{N}{x_0}{x_1}{x_{N - 1}}
    =
    \sum_{n = 0}^{N - 1}
    x_n
    \twiddle{- 2 \pi}{n k}{N},

and its inverse transform:

.. math::

    x_n
    =
    \idft{n}{N}{X_0}{X_1}{X_{N - 1}}
    =
    \frac{1}{N}
    \sum_{k = 0}^{N - 1}
    X_k
    \twiddle{2 \pi}{n k}{N}.

Note that these relations satisfy the Hermitian symmetry :math:`X_{N - k} = X_k^*`.

.. derivation_disclosure::

    .. math::

        X_{N - k}
        &
        =
        \sum_{n = 0}^{N - 1}
        x_n
        \twiddle{- 2 \pi}{n \left( N - k \right)}{N}

        &
        =
        \sum_{n = 0}^{N - 1}
        x_n
        \exp \left( - 2 \pi n I \right)
        \twiddle{2 \pi}{n k}{N}

        &
        =
        \sum_{n = 0}^{N - 1}
        x_n
        \twiddle{2 \pi}{n k}{N}

        &
        =
        \left[
            \sum_{n = 0}^{N - 1}
            x_n
            \twiddle{- 2 \pi}{n k}{N}
        \right]^*
        \,\, \left( \because x_n^* = x_n \right)

        &
        =
        X_k^*.

This symmetry can facilitate the reduction of the computational load, along with the fact that :math:`x_n \in \mathbb{R}`.

