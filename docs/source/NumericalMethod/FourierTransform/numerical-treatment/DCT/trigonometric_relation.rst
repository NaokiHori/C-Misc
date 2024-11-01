The relation

.. math::

    \ctwiddle{
        2 \pi
    }{
        \left( m + \frac{1}{2} \right)
        \left( 2 k + l \right)
    }{
        2 N
    }
    =
    \left( - 1 \right)^l
    \ctwiddle{
        2 \pi
    }{
        \left( n + \frac{1}{2} \right)
        \left( 2 k + l \right)
    }{
        2 N
    }

where :math:`m \equiv N - 1 - n` is repeatedly used in this section, where the symbols :math:`n, k, l` are all integers.

.. derivation_disclosure::

    .. math::

        \ctwiddle{
            2 \pi
        }{
            \left( m + \frac{1}{2} \right)
            \left( 2 k + l \right)
        }{
            2 N
        }
        &
        =
        \ctwiddle{
            2 \pi
        }{
            \left\{ \left( N - 1 - n \right) + \frac{1}{2} \right\}
            \left( 2 k + l \right)
        }{
            2 N
        }

        &
        =
        \ctwiddle{
            2 \pi
        }{
            \left\{ N - \left( n + \frac{1}{2} \right) \right\}
            \left( 2 k + l \right)
        }{
            2 N
        }

        &
        =
        \cos
        \left(
            \left( 2 k + l \right) \pi
            -
            2
            \pi
            \frac{
                \left( n + \frac{1}{2} \right)
                \left( 2 k + l \right)
            }{
                2 N
            }
        \right).

    By utilizing

    .. math::

        \cos \left( \alpha - \beta \right)
        &
        =
        \cos \alpha \cos \beta
        +
        \sin \alpha \sin \beta,

        \cos \left( \left( 2 k + l \right) \pi \right)
        &
        =
        \left( - 1 \right)^l,

        \sin \left( \left( 2 k + l \right) \pi \right)
        &
        =
        0,

    we end up with

    .. math::

        \ctwiddle{
            2 \pi
        }{
            \left( m + \frac{1}{2} \right)
            \left( 2 k + l \right)
        }{
            2 N
        }
        =
        \left( - 1 \right)^l
        \ctwiddle{
            2 \pi
        }{
            \left( n + \frac{1}{2} \right)
            \left( 2 k + l \right)
        }{
            2 N
        }.

