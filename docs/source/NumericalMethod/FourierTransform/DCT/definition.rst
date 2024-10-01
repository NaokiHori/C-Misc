##########
Definition
##########

For a sequence of real numbers :math:`x_n` where :math:`\seq{n}{N - 1}` with :math:`N` being the degree of freedom, the discrete cosine transform (`DCT`) of type 2 is defined as

.. math::

    \defdct,

while its inverse transform (`DCT` of type 3) is defined as

.. math::

    x_n
    \equiv
    \hat{X}_0
    +
    2
    \sum_{k = 1}^{N - 1}
    \hat{X}_k
    \twiddle{\left( 2 n + 1 \right) k}{2 N}
    \equiv
    \idct{N}{n}{\hat{X}_0}{\hat{X}_1}{\hat{X}_{N - 1}}.

Note that these definitions follow `FFTW3 <https://www.fftw.org/fftw3_doc/1d-Real_002deven-DFTs-_0028DCTs_0029.html>`_.

Regarding the inverse transform, for notational convenience, we define :math:`X_k` as

.. math::

    X_k
    \equiv
    \begin{cases}
        \frac{1}{2} \hat{X}_k & k = 0, \\
        \hat{X}_k & \text{otherwise},
    \end{cases}

simplifying the `iDCT`:

.. math::

    \defidct.

