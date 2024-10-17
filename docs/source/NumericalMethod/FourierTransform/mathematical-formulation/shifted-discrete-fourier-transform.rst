##################################
Shifted discrete Fourier transform
##################################

By taking the sampling points differently:

.. math::

    x_n
    =
    \frac{
        2 n + 1
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
    &
    =
    \sum_{n = 0}^{N - 1}
    f_n
    \twiddle{- 2 \pi}{k \left( n + \frac{1}{2} \right)}{N},

    f_n
    &
    =
    \frac{1}{N}
    \sum_{k = 0}^{N - 1}
    F_k
    \twiddle{2 \pi}{k \left( n + \frac{1}{2} \right)}{N},

which is known as `the shifted discrete Fourier transform <https://en.wikipedia.org/wiki/Discrete_Fourier_transform#Generalized_DFT_(shifted_and_non-linear_phase)>`_ and appears when dealing with symmetric signals.

.. seealso::

    * :ref:`Discrete cosine transform <discrete_cosine_transform>`
    * :ref:`Discrete sine transform <discrete_sine_transform>`

