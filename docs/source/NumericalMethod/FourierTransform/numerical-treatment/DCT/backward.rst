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
    \ctwiddle{2 \pi}{\left( n + \frac{1}{2} \right) k}{2 N}.

We perform this conversion a priori:

.. myliteralinclude:: /../../NumericalMethod/FourierTransform/DCT/Lee1984/src/dct.c
    :language: c
    :tag: normalize 0-th wave number before executing DCT3

Furthermore, we introduce a symbol denoting the inverse transform for brevity:

.. math::

    \dctiii{n}{N}{X_0}{X_1}{X_{N - 1}}
    \equiv
    \frac{1}{N}
    \sum_{k = 0}^{N - 1}
    X_k
    \ctwiddle{2 \pi}{\left( n + \frac{1}{2} \right) k}{2 N}.

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
    \frac{1}{N}
    \sum_{k = 0}^{N / 2 - 1}
    X_{2 k}
    \ctwiddle{2 \pi}{\left( n + \frac{1}{2} \right) \left( 2 k \right)}{2 N}
    -
    \frac{1}{N}
    \sum_{k = 0}^{N / 2 - 1}
    X_{2 k + 1}
    \ctwiddle{2 \pi}{\left( n + \frac{1}{2} \right) \left( 2 k + 1 \right)}{2 N}

    &
    =
    \dctiii{n}{N / 2}{X_0}{X_2}{X_{N - 2}}
    -
    \frac{1}{N}
    \sum_{k = 0}^{N / 2 - 1}
    X_{2 k + 1}
    \ctwiddle{2 \pi}{\left( n + \frac{1}{2} \right) \left( 2 k + 1 \right)}{2 N},

due to :ref:`one of the trigonometric relations derived before <trig_relation_phase>`.

To process the second terms:

.. math::

    \frac{1}{N}
    \sum_{k = 0}^{N / 2 - 1}
    X_{2 k + 1}
    \ctwiddle{2 \pi}{\left( n + \frac{1}{2} \right) \left( 2 k + 1 \right)}{2 N},

we assign

.. math::

    \alpha
    =
    \alpha \left( n, k \right)
    =
    2 \pi \frac{\left( n + \frac{1}{2} \right) \left( 2 k + 1 \right)}{2 N}

and

.. math::

    \beta
    =
    \beta \left( n \right)
    =
    2 \pi \frac{n + \frac{1}{2}}{2 N}

to :ref:`one of the trigonometric relations derived before <trig_relation_prod_sum>`, allowing us to reformulate the second part as

.. math::

    &
    \frac{1}{N}
    \sum_{k = 0}^{N / 2 - 1}
    X_{2 k + 1}
    \ctwiddle{2 \pi}{\left( n + \frac{1}{2} \right) \left( 2 k + 1 \right)}{2 N}

    =
    &
    \frac{1}{N}
    \sum_{k = 0}^{N / 2 - 1}
    X_{2 k + 1}
    \cos \alpha

    =
    &
    \frac{1}{2 N \cos \beta}
    \sum_{k = 1}^{N / 2}
    X_{2 k - 1}
    \ctwiddle{2 \pi}{\left( n + \frac{1}{2} \right) \left( 2 k \right)}{2 N}
    +
    \frac{1}{2 N \cos \beta}
    \sum_{k = 0}^{N / 2 - 1}
    X_{2 k + 1}
    \ctwiddle{2 \pi}{\left( n + \frac{1}{2} \right) \left( 2 k \right)}{2 N}.

Not only the second but the first term seemingly satisfy the definition of the inverse transform, which is not true since the summation range is different: it should be from :math:`k = 0` to :math:`N / 2 - 1`.
For :math:`k = 0`, we are free to add :math:`k = 0` to the summation by artificially defining :math:`X_{-1} \equiv 0`.
Regarding :math:`k = N / 2`, which needs to be removed, we notice

.. math::

    X_{N - 1}
    \ctwiddle{2 \pi}{\left( n + \frac{1}{2} \right) N}{2 N}
    =
    X_{N - 1}
    \cos \left( \frac{2 n + 1}{2} \pi \right)
    =
    0,

and thus the first term leads to

.. math::

    \frac{1}{2 N \cos \beta}
    \sum_{k = 0}^{N / 2 - 1}
    X_{2 k - 1}
    \ctwiddle{2 \pi}{\left( n + \frac{1}{2} \right) \left( 2 k \right)}{2 N},

and in total we find

.. math::

    \frac{1}{N}
    \sum_{k = 0}^{N / 2 - 1}
    X_{2 k + 1}
    \ctwiddle{2 \pi}{\left( n + \frac{1}{2} \right) \left( 2 k + 1 \right)}{2 N}
    =
    &
    =
    \frac{1}{2 N \cos \beta}
    \sum_{k = 0}^{N / 2 - 1}
    \left(
        X_{2 k - 1}
        +
        X_{2 k + 1}
    \right)
    \ctwiddle{2 \pi}{\left( n + \frac{1}{2} \right) \left( 2 k \right)}{2 N}

    &
    =
    \frac{1}{2 \cos \beta}
    \dctiii{n}{N / 2}{X_1}{X_1 + X_3}{X_{N / 2 - 3} + X_{N / 2 - 1}}.

In summary, we obtain the following recurrence relation:

.. math::

    x_n
    &
    =
    \dctiii{n}{N / 2}{X_0}{X_2}{X_{N / 2 - 2}}
    +
    \frac{1}{2 \cos \beta}
    \dctiii{n}{N / 2}{X_1}{X_1 + X_3}{X_{N / 2 - 3} + X_{N / 2 - 1}},

    x_{N - 1 - n}
    &
    =
    \dctiii{n}{N / 2}{X_0}{X_2}{X_{N / 2 - 2}}
    -
    \frac{1}{2 \cos \beta}
    \dctiii{n}{N / 2}{X_1}{X_1 + X_3}{X_{N / 2 - 3} + X_{N / 2 - 1}},

with :math:`\seq{n}{N / 2 - 1}`.

.. myliteralinclude:: /../../NumericalMethod/FourierTransform/DCT/Lee1984/src/dct.c
    :language: c
    :tag: divide and conquer, backward

Also we let

.. math::

    \beta
    \equiv
    2
    \pi
    \frac{
        n + \frac{1}{2}
    }{
        2 N
    }.

