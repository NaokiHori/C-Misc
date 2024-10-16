By taking the sampling points differently:

.. math::

    x_n
    =
    \frac{
        2 n
        +
        1
    }{
        2
    }
    \frac{
        L
    }{
        N
    },

we obtain

.. math::

    F_k
    =
    \frac{1}{N}
    \sum_{n = 0}^{N - 1}
    f_n
    \exp
    \left\{
        -
        \frac{
            2 \pi k \left( n + \frac{1}{2} \right)
        }{
            N
        }
        I
    \right\},

and

.. math::

    f_n
    =
    \sum_{k = 0}^{N - 1}
    F_k
    \exp
    \left\{
        \frac{
            2 \pi k \left( n + \frac{1}{2} \right)
        }{
            N
        }
        I
    \right\},

which is known as `the shifted DFT <https://en.wikipedia.org/wiki/Discrete_Fourier_transform#Generalized_DFT_(shifted_and_non-linear_phase)>`_ and appears when dealing with symmetric signals.

