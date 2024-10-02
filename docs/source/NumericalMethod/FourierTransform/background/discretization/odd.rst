####################
Odd-symmetric signal
####################

We consider a special signal satisfying

.. math::

    f_{2 N - 1 - n}
    \equiv
    -
    f_{n};

namely the signal is periodic and is anti-symmetric with respect to :math:`x = L / 2`.

The forward transform leads to

.. math::

    F_k
    &
    =
    \frac{1}{2 N}
    \sum_{n = 0}^{2 N - 1}
    f_n
    \exp
    \left\{
        -
        \frac{
            \pi k \left( n + \frac{1}{2} \right)
        }{
            N
        }
        I
    \right\}

    &
    =
    \frac{1}{2 N}
    \sum_{n = 0}^{N - 1}
    f_n
    \exp
    \left\{
        -
        \frac{
            \pi k \left( n + \frac{1}{2} \right)
        }{
            N
        }
        I
    \right\}
    +
    \frac{1}{2 N}
    \sum_{n = N}^{2 N - 1}
    f_n
    \exp
    \left\{
        -
        \frac{
            \pi k \left( n + \frac{1}{2} \right)
        }{
            N
        }
        I
    \right\}

    &
    =
    \frac{1}{2 N}
    \sum_{n = 0}^{N - 1}
    f_n
    \exp
    \left\{
        -
        \frac{
            \pi k \left( n + \frac{1}{2} \right)
        }{
            N
        }
        I
    \right\}
    +
    \frac{1}{2 N}
    \sum_{m = N - 1}^{0}
    f_{2 N - 1 - m}
    \exp
    \left\{
        -
        \frac{
            \pi k \left( 2 N - 1 - m + \frac{1}{2} \right)
        }{
            N
        }
        I
    \right\}
    \,\,
    \left( m \equiv 2 N - 1 - n \right)

    &
    =
    \frac{1}{2 N}
    \sum_{n = 0}^{N - 1}
    f_n
    \exp
    \left\{
        -
        \frac{
            \pi k \left( n + \frac{1}{2} \right)
        }{
            N
        }
        I
    \right\}
    -
    \frac{1}{2 N}
    \sum_{n = 0}^{N - 1}
    f_{n}
    \exp
    \left\{
        -
        \frac{
            \pi k \left( 2 N - n - \frac{1}{2} \right)
        }{
            N
        }
        I
    \right\}

    &
    =
    \frac{1}{2 N}
    \sum_{n = 0}^{N - 1}
    f_n
    \exp
    \left\{
        -
        \frac{
            \pi k \left( n + \frac{1}{2} \right)
        }{
            N
        }
        I
    \right\}
    -
    \frac{1}{2 N}
    \sum_{n = 0}^{N - 1}
    f_{n}
    \exp
    \left\{
        \frac{
            \pi k \left( n + \frac{1}{2} \right)
        }{
            N
        }
        I
    \right\}

    &
    =
    -
    \frac{I}{N}
    \sum_{n = 0}^{N - 1}
    f_n
    \sin
    \left\{
        \frac{
            \pi k \left( n + \frac{1}{2} \right)
        }{
            N
        }
    \right\},

which is known as `the discrete sine transform <https://en.wikipedia.org/wiki/Discrete_sine_transform#DST-II>`_.

The backward transform, on the other hand, leads to

.. math::

    TBA

