#####################
Even-symmetric signal
#####################

We consider a special signal satisfying

.. math::

    f_{2 N - 1 - n}
    \equiv
    f_{n};

namely the signal is periodic and is symmetric with respect to :math:`x = L / 2`.

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
    +
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
    +
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
    \frac{1}{N}
    \sum_{n = 0}^{N - 1}
    f_n
    \cos
    \frac{
        \pi k \left( n + \frac{1}{2} \right)
    }{
        N
    },

which is known as `the discrete cosine transform <https://en.wikipedia.org/wiki/Discrete_cosine_transform#DCT-II>`_.

To find the backward transform, we consider

.. math::

    \sum_{k = 0}^{N - 1}
    F_k
    \cos
    \frac{
        \pi k \left( n + \frac{1}{2} \right)
    }{
        N
    },

which, by using the forward transform, can be written as

.. math::

    &
    \sum_{k = 0}^{N - 1}
    \frac{1}{N}
    \sum_{m = 0}^{N - 1}
    f_m
    \cos
    \frac{
        \pi k \left( m + \frac{1}{2} \right)
    }{
        N
    }
    \cos
    \frac{
        \pi k \left( n + \frac{1}{2} \right)
    }{
        N
    }

    =
    &
    \frac{1}{N}
    \sum_{m = 0}^{N - 1}
    f_m
    \sum_{k = 0}^{N - 1}
    \cos
    \frac{
        \pi k \left( m + \frac{1}{2} \right)
    }{
        N
    }
    \cos
    \frac{
        \pi k \left( n + \frac{1}{2} \right)
    }{
        N
    }

    =
    &
    \frac{1}{N}
    \sum_{m = 0}^{N - 1}
    f_m
    \sum_{k = 0}^{N - 1}
    \left\{
        \frac{1}{2}
        \cos
        \frac{
            \pi k \left( m + n + 1 \right)
        }{
            N
        }
        +
        \frac{1}{2}
        \cos
        \frac{
            \pi k \left( m - n \right)
        }{
            N
        }
    \right\}.

