We consider :ref:`the inverse transform (discrete sine transform of type III) <discrete_sine_transform>`:

.. math::

    x_n
    =
    \frac{1}{N}
    \left\{
        \sum_{k = 0}^{N - 2}
        \hat{X}_k
        \stwiddle{2 \pi}{\left( n + \frac{1}{2} \right) \left( k + 1 \right)}{2 N}
        +
        \left( - 1 \right)^n
        \frac{\hat{X}_{N - 1}}{2}
    \right\}

for :math:`\seq{n}{0}{1}{N - 1}`.
For later convenience, we define

.. math::

    X_k
    =
    \begin{cases}
        \frac{\hat{X}_{N - 1}}{2} & k = N - 1, \\
        \hat{X}_k & \text{otherwise},
    \end{cases}

to write the inverse transform as

.. math::

    x_n
    =
    \frac{1}{N}
    \sum_{k = 0}^{N - 1}
    X_k
    \stwiddle{2 \pi}{\left( n + \frac{1}{2} \right) \left( k + 1 \right)}{2 N}
    \equiv
    \dstiii{n}{N}{X_0}{X_1}{X_{N - 1}}

for :math:`\seq{n}{0}{1}{N - 1}`.

Note that we perform this conversion a priori:

.. myliteralinclude:: /../../NumericalMethod/FourierTransform/DST/src/dst.c
    :language: c
    :tag: normalize (N-1)-th wave number before executing DST3

Assuming that :math:`N` is a multiple of :math:`2`, we decompose the right-hand-side terms into even and odd components:

.. math::

    x_n
    &
    =
    \frac{1}{N}
    \sum_{k = 0}^{N / 2 - 1}
    X_{2 k}
    \stwiddle{2 \pi}{\left( n + \frac{1}{2} \right) \left( 2 k + 1 \right)}{2 N}
    +
    \frac{1}{N}
    \sum_{k = 0}^{N / 2 - 1}
    X_{2 k + 1}
    \stwiddle{2 \pi}{\left( n + \frac{1}{2} \right) \left( 2 k + 2 \right)}{2 N}.

    &
    =
    \frac{1}{N}
    \sum_{k = 0}^{N / 2 - 1}
    X_{2 k}
    \stwiddle{2 \pi}{\left( n + \frac{1}{2} \right) \left( 2 k + 1 \right)}{2 N}
    +
    \frac{1}{2}
    \dstiii{n}{N / 2}{X_1}{X_3}{X_{N - 1}}.

By introducing :math:`m = N - 1 - n` to utilize the symmetric nature of :math:`x_n`, we see

.. math::

    x_{N - 1 - n}
    &
    =
    \frac{1}{N}
    \sum_{k = 0}^{N / 2 - 1}
    X_{2 k}
    \stwiddle{2 \pi}{\left( m + \frac{1}{2} \right) \left( 2 k + 1 \right)}{2 N}
    +
    \frac{1}{N}
    \sum_{k = 0}^{N / 2 - 1}
    X_{2 k + 1}
    \stwiddle{2 \pi}{\left( m + \frac{1}{2} \right) \left( 2 k + 2 \right)}{2 N}

    &
    =
    \frac{1}{N}
    \sum_{k = 0}^{N / 2 - 1}
    X_{2 k}
    \stwiddle{2 \pi}{\left( n + \frac{1}{2} \right) \left( 2 k + 1 \right)}{2 N}
    -
    \frac{1}{2}
    \dstiii{n}{N / 2}{X_1}{X_3}{X_{N - 1}}.

By adopting the product-to-sum identity:

.. math::

    \sin \alpha
    \equiv
    \frac{
        \sin \left( \alpha + \beta \right)
        +
        \sin \left( \alpha - \beta \right)
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

the first term

.. math::

    \frac{1}{N}
    \sum_{k = 0}^{N / 2 - 1}
    X_{2 k}
    \stwiddle{2 \pi}{\left( n + \frac{1}{2} \right) \left( 2 k + 1 \right)}{2 N}

leads to

.. math::

    &
    \frac{1}{2 \cos \beta}
    \frac{1}{N}
    \sum_{k = 0}^{N / 2 - 1}
    X_{2 k}
    \stwiddle{2 \pi}{\left( n + \frac{1}{2} \right) \left( 2 k \right)}{2 N}
    +
    \frac{1}{2 \cos \beta}
    \frac{1}{N}
    \sum_{k = 0}^{N / 2 - 1}
    X_{2 k}
    \stwiddle{2 \pi}{\left( n + \frac{1}{2} \right) \left( 2 k + 2 \right)}{2 N}

    =
    &
    \frac{1}{4 \cos \beta}
    \frac{1}{N / 2}
    \sum_{k = - 1}^{N / 2 - 2}
    X_{2 k + 2}
    \stwiddle{2 \pi}{\left( n + \frac{1}{2} \right) \left( k + 1 \right)}{2 \left( N / 2 \right)}
    +
    \frac{1}{4 \cos \beta}
    \dstiii{n}{N / 2}{X_0}{X_2}{X_{N - 2}}

    =
    &
    \frac{1}{4 \cos \beta}
    \dstiii{n}{N / 2}{X_2}{X_4}{X_N}
    +
    \frac{1}{4 \cos \beta}
    \dstiii{n}{N / 2}{X_0}{X_2}{X_{N - 2}}

    =
    &
    \frac{1}{4 \cos \beta}
    \dstiii{n}{N / 2}{X_0 + X_2}{X_2 + X_4}{X_{N - 2} + X_N},

where we let :math:`X_N = 0`.

In summary, we obtain the following recurrence relation:

.. math::

    x_n
    &
    =
    \frac{1}{4 \cos \beta}
    \dstiii{n}{N / 2}{X_0 + X_2}{X_2 + X_4}{X_{N - 2} + X_N}
    +
    \frac{1}{2}
    \dstiii{n}{N / 2}{X_1}{X_3}{X_{N - 1}},

    x_{N - 1 - n}
    &
    =
    \frac{1}{4 \cos \beta}
    \dstiii{n}{N / 2}{X_0 + X_2}{X_2 + X_4}{X_{N - 2} + X_N}
    -
    \frac{1}{2}
    \dstiii{n}{N / 2}{X_1}{X_3}{X_{N - 1}},

for :math:`\seq{n}{0}{1}{N / 2 - 1}`.
Note that :math:`x_0 = X_0` for :math:`N = 1`.

.. myliteralinclude:: /../../NumericalMethod/FourierTransform/DST/src/dst.c
    :language: c
    :tag: backward transform

