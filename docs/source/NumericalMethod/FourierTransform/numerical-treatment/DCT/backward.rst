We consider :ref:`the inverse transform (discrete cosine transform of type III) <discrete_cosine_transform>`:

.. math::

    x_n
    =
    \frac{1}{2 N}
    \left\{
        \hat{X}_0
        +
        2
        \sum_{k = 1}^{N - 1}
        \hat{X}_k
        \ctwiddle{2 \pi}{\left( n + \frac{1}{2} \right) k}{2 N}
    \right\}.

For notational simplicity, we define

.. math::

    X_k
    =
    \begin{cases}
        \frac{1}{2} \hat{X}_0 & k = 0, \\
        \hat{X}_k & \text{otherwise},
    \end{cases}

to write the inverse transform as

.. math::

    x_n
    =
    \frac{1}{N}
    \sum_{k = 0}^{N - 1}
    X_k
    \ctwiddle{2 \pi}{\left( n + \frac{1}{2} \right) k}{2 N}
    \equiv
    \dctiii{n}{N}{X_0}{X_1}{X_{N - 1}}.

Note that we perform this conversion a priori:

.. myliteralinclude:: /../../NumericalMethod/FourierTransform/DCT/Lee1984/src/dct.c
    :language: c
    :tag: normalize 0-th wave number before executing DCT3

Assuming that :math:`N` is a multiple of :math:`2`, we decompose the right-hand side into two components:

.. math::

    x_n
    &
    =
    \frac{1}{N}
    \sum_{k = 0}^{N / 2 - 1}
    X_{2 k}
    \ctwiddle{2 \pi}{\left( n + \frac{1}{2} \right) \left( 2 k \right)}{2 N}
    +
    \frac{1}{N}
    \sum_{k = 0}^{N / 2 - 1}
    X_{2 k + 1}
    \ctwiddle{2 \pi}{\left( n + \frac{1}{2} \right) \left( 2 k + 1 \right)}{2 N}

    &
    =
    \frac{1}{2}
    \frac{1}{N / 2}
    \sum_{k = 0}^{N / 2 - 1}
    X_{2 k}
    \ctwiddle{2 \pi}{\left( n + \frac{1}{2} \right) k}{2 \left( N / 2 \right)}
    +
    \frac{1}{N}
    \sum_{k = 0}^{N / 2 - 1}
    X_{2 k + 1}
    \ctwiddle{2 \pi}{\left( n + \frac{1}{2} \right) \left( 2 k + 1 \right)}{2 N}

    &
    =
    \frac{1}{2}
    \dctiii{n}{N / 2}{X_0}{X_2}{X_{N - 2}}
    +
    \frac{1}{N}
    \sum_{k = 0}^{N / 2 - 1}
    X_{2 k + 1}
    \ctwiddle{2 \pi}{\left( n + \frac{1}{2} \right) \left( 2 k + 1 \right)}{2 N}.

To decompose :math:`n` as well, we consider :math:`n \leftarrow N - 1 - n` to yield

.. math::

    x_{N - 1 - n}
    &
    =
    \frac{1}{N}
    \sum_{k = 0}^{N / 2 - 1}
    X_{2 k}
    \ctwiddle{2 \pi}{\left( N - n - \frac{1}{2} \right) \left( 2 k \right)}{2 N}
    +
    \frac{1}{N}
    \sum_{k = 0}^{N / 2 - 1}
    X_{2 k + 1}
    \ctwiddle{2 \pi}{\left( N - n - \frac{1}{2} \right) \left( 2 k + 1 \right)}{2 N}

    &
    =
    \frac{1}{2}
    \frac{1}{N / 2}
    \sum_{k = 0}^{N / 2 - 1}
    X_{2 k}
    \ctwiddle{2 \pi}{\left( n + \frac{1}{2} \right) k}{2 \left( N / 2 \right)}
    -
    \frac{1}{N}
    \sum_{k = 0}^{N / 2 - 1}
    X_{2 k + 1}
    \ctwiddle{2 \pi}{\left( n + \frac{1}{2} \right) \left( 2 k + 1 \right)}{2 N}

    &
    =
    \frac{1}{2}
    \dctiii{n}{N / 2}{X_0}{X_2}{X_{N - 2}}
    -
    \frac{1}{N}
    \sum_{k = 0}^{N / 2 - 1}
    X_{2 k + 1}
    \ctwiddle{2 \pi}{\left( n + \frac{1}{2} \right) \left( 2 k + 1 \right)}{2 N},

due to :ref:`one of the trigonometric relations derived before <dct_trigonometric_relation>`.
By adopting the product-to-sum identity:

.. math::

    \cos \alpha
    \equiv
    \frac{
        \cos \left( \alpha + \beta \right)
        +
        \cos \left( \alpha - \beta \right)
    }{
        2 \cos \beta
    }

with

.. math::

    \beta
    =
    2 \pi
    \frac{
        n + \frac{1}{2}
    }{
        2 N
    },

we obtain

.. math::

    &
    \frac{1}{N}
    \sum_{k = 0}^{N / 2 - 1}
    X_{2 k + 1}
    \ctwiddle{2 \pi}{\left( n + \frac{1}{2} \right) \left( 2 k + 1 \right)}{2 N}

    =
    &
    \frac{1}{2 N \cos \beta}
    \sum_{k = 0}^{N / 2 - 1}
    X_{2 k + 1}
    \ctwiddle{2 \pi}{\left( n + \frac{1}{2} \right) \left( 2 k \right)}{2 N}
    +
    \frac{1}{2 N \cos \beta}
    \sum_{k = 1}^{N / 2}
    X_{2 k - 1}
    \ctwiddle{2 \pi}{\left( n + \frac{1}{2} \right) \left( 2 k \right)}{2 N}.

The first term leads to

.. math::

    \frac{1}{4 \cos \beta}
    \frac{1}{N / 2}
    \sum_{k = 0}^{N / 2 - 1}
    X_{2 k + 1}
    \ctwiddle{2 \pi}{\left( n + \frac{1}{2} \right) k}{2 \left( N / 2 \right)}
    =
    \frac{1}{4 \cos \beta}
    \dctiii{n}{N / 2}{X_1}{X_3}{X_{N - 1}},

while the second term is

.. math::

    &
    \frac{1}{4 \cos \beta}
    \frac{1}{N / 2}
    \sum_{k = 0}^{N / 2 - 1}
    X_{2 k - 1}
    \ctwiddle{2 \pi}{\left( n + \frac{1}{2} \right) k}{2 \left( N / 2 \right)}
    -
    \frac{1}{2 N \cos \beta}
    X_{-1}
    +
    \frac{1}{2 N \cos \beta}
    X_{N - 1}
    \ctwiddle{2 \pi}{\left( n + \frac{1}{2} \right) N}{2 N}

    =
    &
    \frac{1}{4 \cos \beta}
    \dctiii{n}{N / 2}{X_{-1}}{X_1}{X_{N - 3}},

where we artificially specify :math:`X_{-1} = 0`.
As a consequence, we obtain

.. math::

    \frac{1}{N}
    \sum_{k = 0}^{N / 2 - 1}
    X_{2 k + 1}
    \ctwiddle{2 \pi}{\left( n + \frac{1}{2} \right) \left( 2 k + 1 \right)}{2 N}
    =
    \frac{1}{4 \cos \beta}
    \dctiii{n}{N / 2}{X_1}{X_1 + X_3}{X_{N - 3} + X_{N - 1}}.

In summary, we obtain the following recurrence relation:

.. math::

    x_n
    &
    =
    \frac{1}{2}
    \dctiii{n}{N / 2}{X_0}{X_2}{X_{N - 2}}
    +
    \frac{1}{4 \cos \beta}
    \dctiii{n}{N / 2}{X_1}{X_1 + X_3}{X_{N - 3} + X_{N - 1}},

    x_{N - 1 - n}
    &
    =
    \frac{1}{2}
    \dctiii{n}{N / 2}{X_0}{X_2}{X_{N - 2}}
    -
    \frac{1}{4 \cos \beta}
    \dctiii{n}{N / 2}{X_1}{X_1 + X_3}{X_{N - 3} + X_{N - 1}},

with :math:`\seq{n}{N / 2 - 1}`.
Note that :math:`x_0 = X_0` for :math:`N = 1`.

.. myliteralinclude:: /../../NumericalMethod/FourierTransform/DCT/Lee1984/src/dct.c
    :language: c
    :tag: backward transform

