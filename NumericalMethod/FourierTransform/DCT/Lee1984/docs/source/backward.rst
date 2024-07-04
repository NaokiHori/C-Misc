##################
Backward transform
##################

The `iDCT` is defined as

.. math::

    \defidct.

Assuming that :math:`N` is a multiple of :math:`2`, we decompose the right-hand side into two components:

.. math::

    x_n
    =
    2
    \sum_{k = 0}^{N / 2 - 1}
    X_{2 k}
    \twiddle{\left( 2 n + 1 \right) \left( 2 k \right)}{2 N}
    +
    2
    \sum_{k = 0}^{N / 2 - 1}
    X_{2 k + 1}
    \twiddle{\left( 2 n + 1 \right) \left( 2 k + 1 \right)}{2 N}.

To decompose :math:`n` as well, we consider :math:`n \leftarrow N - 1 - n` to yield

.. math::

    x_{N - 1 - n}
    &
    =
    2
    \sum_{k = 0}^{N / 2 - 1}
    X_{2 k}
    \twiddle{\left\{ 2 \left( N - 1 - n \right) + 1 \right\} \left( 2 k \right)}{2 N}
    +
    2
    \sum_{k = 0}^{N / 2 - 1}
    X_{2 k + 1}
    \twiddle{\left\{ 2 \left( N - 1 - n \right) + 1 \right\} \left( 2 k + 1 \right)}{2 N}

    &
    =
    2
    \sum_{k = 0}^{N / 2 - 1}
    X_{2 k}
    \twiddle{\left( 2 n + 1 \right) \left( 2 k \right)}{2 N}
    -
    2
    \sum_{k = 0}^{N / 2 - 1}
    X_{2 k + 1}
    \twiddle{\left( 2 n + 1 \right) \left( 2 k + 1 \right)}{2 N},

due to :ref:`one of the trigonometric relations derived before <trig_relation_phase>`.

Thus we notice

.. math::

    x_n
    &
    =
    \idct{N / 2}{n}{X_0}{X_2}{X_{N / 2 - 2}}
    +
    2
    \sum_{k = 0}^{N / 2 - 1}
    X_{2 k + 1}
    \twiddle{\left( 2 n + 1 \right) \left( 2 k + 1 \right)}{2 N},

    x_{N - 1 - n}
    &
    =
    \idct{N / 2}{n}{X_0}{X_2}{X_{N / 2 - 2}}
    -
    2
    \sum_{k = 0}^{N / 2 - 1}
    X_{2 k + 1}
    \twiddle{\left( 2 n + 1 \right) \left( 2 k + 1 \right)}{2 N}.

Although the first part is written as an `iDCT`, the second part cannot be reformulated due to the phase shift, preventing the application of the divide-and-conquer strategy.
To make it `iDCT` as well, we assign

.. math::

    \alpha
    =
    \alpha \left( n, k \right)
    =
    \frac{\pi \left( 2 n + 1 \right) \left( 2 k + 1 \right)}{2 N}

and

.. math::

    \beta
    =
    \beta \left( n \right)
    =
    \frac{\pi \left( 2 n + 1 \right)}{2 N}

to :ref:`one of the trigonometric relations derived before <trig_relation_prod_sum>`, giving

.. math::

    2
    \sum_{k = 0}^{N / 2 - 1}
    X_{2 k + 1}
    \twiddle{\left( 2 n + 1 \right) \left( 2 k + 1 \right)}{2 N}
    =
    &
    2
    \sum_{k = 0}^{N / 2 - 1}
    X_{2 k + 1}
    \cos \alpha

    =
    &
    \frac{1}{\cos \beta}
    \sum_{k = 1}^{N / 2}
    X_{2 k - 1}
    \twiddle{\left( 2 n + 1 \right) \left( 2 k \right)}{2 N}
    +
    \frac{1}{\cos \beta}
    \sum_{k = 0}^{N / 2 - 1}
    X_{2 k + 1}
    \twiddle{\left( 2 n + 1 \right) \left( 2 k \right)}{2 N}.

Not only the second but the first term seemingly satisfy the definition of the `iDCT`; this is not true since the summation range is different: it should be from :math:`k = 0` to :math:`N / 2 - 1`.
By defining :math:`X_{-1} \equiv 0`, we are free to add :math:`k = 0` to the summation.
Regarding :math:`k = N / 2` which needs to be removed, we notice

.. math::

    X_{N - 1}
    \twiddle{\left( 2 n + 1 \right) N}{2 N}
    =
    X_{N - 1}
    \cos \left[ \frac{\pi}{2} \left( 2 n + 1 \right) \right]
    =
    0,

and thus the first term leads to

.. math::

    \frac{1}{\cos \beta}
    \sum_{k = 0}^{N / 2 - 1}
    X_{2 k - 1}
    \twiddle{\left( 2 n + 1 \right) \left( 2 k \right)}{2 N},

and in total we find

.. math::

    2
    \sum_{k = 0}^{N / 2 - 1}
    X_{2 k + 1}
    \twiddle{\left( 2 n + 1 \right) \left( 2 k + 1 \right)}{2 N}
    =
    &
    =
    \frac{1}{\cos \beta}
    \sum_{k = 0}^{N / 2 - 1}
    \left(
        X_{2 k - 1}
        +
        X_{2 k + 1}
    \right)
    \twiddle{\left( 2 n + 1 \right) \left( 2 k \right)}{2 N}

    &
    =
    \frac{1}{2 \cos \beta}
    \idct{N / 2}{n}{X_1}{X_1 + X_3}{X_{N / 2 - 3} + X_{N / 2 - 1}}.

In summary, we obtain the following recurrence relation:

.. math::

    x_n
    &
    =
    \idct{N / 2}{n}{X_0}{X_2}{X_{N / 2 - 2}}
    +
    \frac{1}{2 \cos \beta}
    \idct{N / 2}{n}{X_1}{X_1 + X_3}{X_{N / 2 - 3} + X_{N / 2 - 1}},

    x_{N - 1 - n}
    &
    =
    \idct{N / 2}{n}{X_0}{X_2}{X_{N / 2 - 2}}
    -
    \frac{1}{2 \cos \beta}
    \idct{N / 2}{n}{X_1}{X_1 + X_3}{X_{N / 2 - 3} + X_{N / 2 - 1}},

with :math:`\seq{n}{N / 2 - 1}` and

.. math::

    \beta
    \equiv
    \frac{\pi \left( 2 n + 1 \right)}{2 N}.

