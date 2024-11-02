The relation

.. math::

    \stwiddle{
        2 \pi
    }{
        \left( m + \frac{1}{2} \right)
        \left( 2 k + l \right)
    }{
        2 N
    }
    =
    \left( - 1 \right)^{l + 1}
    \stwiddle{
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

        \stwiddle{
            2 \pi
        }{
            \left( m + \frac{1}{2} \right)
            \left( 2 k + l \right)
        }{
            2 N
        }
        &
        =
        \stwiddle{
            2 \pi
        }{
            \left\{ \left( N - 1 - n \right) + \frac{1}{2} \right\}
            \left( 2 k + l \right)
        }{
            2 N
        }

        &
        =
        \stwiddle{
            2 \pi
        }{
            \left\{ N - \left( n + \frac{1}{2} \right) \right\}
            \left( 2 k + l \right)
        }{
            2 N
        }

        &
        =
        \sin
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

        \sin \left( \alpha - \beta \right)
        &
        =
        \sin \alpha \cos \beta
        -
        \cos \alpha \sin \beta,

        \sin \left( \left( 2 k + l \right) \pi \right)
        &
        =
        0,

        \cos \left( \left( 2 k + l \right) \pi \right)
        &
        =
        \left( - 1 \right)^l,

    we end up with

    .. math::

        \stwiddle{
            2 \pi
        }{
            \left( m + \frac{1}{2} \right)
            \left( 2 k + l \right)
        }{
            2 N
        }
        =
        \left( - 1 \right)^{l + 1}
        \stwiddle{
            2 \pi
        }{
            \left( n + \frac{1}{2} \right)
            \left( 2 k + l \right)
        }{
            2 N
        }.

