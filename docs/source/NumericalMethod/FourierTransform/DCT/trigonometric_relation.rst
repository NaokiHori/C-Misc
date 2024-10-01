######################
Trigonometric relation
######################

The following relations of the trigonometric function are repeatedly used and thus they are derived here.

.. _trig_relation_phase:

**********
Relation 1
**********

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

**********
Relation 2
**********

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

