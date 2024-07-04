###
DCT
###

Discrete Cosine Transform by Lee 1984.

**********
Definition
**********

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

***********************
Trigonometric relations
***********************

The following relations of the trigonometric function are repeatedly used and thus they are derived here.

.. _trig_relation_phase:

==========
Relation 1
==========

.. math::

    &
    \cos
    \left[
        \frac{
            \pi
            \left\{ 2 \left( N - 1 - n \right) + 1 \right\}
            \left( 2 k \right)
        }{
            2 N
        }
    \right]
    =
    \cos
    \left[
        \left( 2 k \right) \pi
        -
        \frac{
            \pi
            \left( 2 n + 1 \right)
            \left( 2 k \right)
        }{
            2 N
        }
    \right]
    =
    +
    \cos
    \left[
        \frac{
            \pi
            \left( 2 n + 1 \right)
            \left( 2 k \right)
        }{
            2 N
        }
    \right],

    &
    \cos
    \left[
        \frac{
            \pi
            \left\{ 2 \left( N - 1 - n \right) + 1 \right\}
            \left( 2 k + 1 \right)
        }{
            2 N
        }
    \right]
    =
    \cos
    \left[
        \left( 2 k + 1 \right) \pi
        -
        \frac{
            \pi
            \left( 2 n + 1 \right)
            \left( 2 k + 1 \right)
        }{
            2 N
        }
    \right]
    =
    -
    \cos
    \left[
        \frac{
            \pi
            \left( 2 n + 1 \right)
            \left( 2 k + 1 \right)
        }{
            2 N
        }
    \right].

.. _trig_relation_prod_sum:

==========
Relation 2
==========

.. math::

    2 \cos \alpha
    =
    \frac{
        \cos \left( \alpha + \beta \right)
        +
        \cos \left( \alpha - \beta \right)
    }{\cos \beta}.

In particular, with

.. math::

    \alpha
    =
    \alpha \left( i, j \right)
    =
    \frac{\pi \left( 2 i + 1 \right) \left( 2 j + 1 \right)}{2 N}

and

.. math::

    \beta
    =
    \beta \left( i \right)
    =
    \frac{\pi \left( 2 i + 1 \right)}{2 N},

we notice

.. math::

    2
    \sum_{j = 0}^{N / 2 - 1}
    X_{2 j + 1}
    \twiddle{\left( 2 i + 1 \right) \left( 2 j + 1 \right)}{2 N}
    =
    &
    2
    \sum_{j = 0}^{N / 2 - 1}
    X_{2 j + 1}
    \cos \alpha

    =
    &
    \frac{1}{\cos \beta}
    \sum_{j = 0}^{N / 2 - 1}
    X_{2 j + 1}
    \left[
        \cos \left( \alpha + \beta \right)
        +
        \cos \left( \alpha - \beta \right)
    \right]

    =
    &
    \frac{1}{\cos \beta}
    \sum_{j = 0}^{N / 2 - 1}
    X_{2 j + 1}
    \twiddle{\left( 2 i + 1 \right) \left( 2 j + 2 \right)}{2 N}
    +
    \frac{1}{\cos \beta}
    \sum_{j = 0}^{N / 2 - 1}
    X_{2 j + 1}
    \twiddle{\left( 2 i + 1 \right) \left( 2 j     \right)}{2 N}

    =
    &
    \frac{1}{\cos \beta}
    \sum_{l = 1}^{N / 2}
    X_{2 l - 1}
    \twiddle{\left( 2 i + 1 \right) \left( 2 l \right)}{2 N}
    +
    \frac{1}{\cos \beta}
    \sum_{j = 0}^{N / 2 - 1}
    X_{2 j + 1}
    \twiddle{\left( 2 i + 1 \right) \left( 2 j \right)}{2 N}

    =
    &
    \frac{1}{\cos \beta}
    \sum_{j = 1}^{N / 2}
    X_{2 j - 1}
    \twiddle{\left( 2 i + 1 \right) \left( 2 j \right)}{2 N}
    +
    \frac{1}{\cos \beta}
    \sum_{j = 0}^{N / 2 - 1}
    X_{2 j + 1}
    \twiddle{\left( 2 i + 1 \right) \left( 2 j \right)}{2 N}.

.. toctree::

    forward.rst
    backward.rst

