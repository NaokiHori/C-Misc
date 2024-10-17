The forward transform (discrete cosine transform of type II) is defined as

.. math::

    X_k
    \equiv
    2
    \sum_{n = 0}^{N - 1}
    x_n
    \ctwiddle{2 \pi}{\left( n + \frac{1}{2} \right) k}{2 N}
    \equiv
    \dct{N}{k}{x_0}{x_1}{x_{N - 1}}.

Assuming that :math:`N` is a multiple of :math:`2`, we consider the even and odd wavenumbers separately:

.. math::

    X_{2 k}
    =
    &
    2
    \sum_{n = 0}^{N - 1}
    x_n
    \ctwiddle{2 \pi}{\left( n + \frac{1}{2} \right) \left( 2 k \right)}{2 N},

    X_{2 k + 1}
    =
    &
    2
    \sum_{n = 0}^{N - 1}
    x_n
    \ctwiddle{2 \pi}{\left( n + \frac{1}{2} \right) \left( 2 k + 1 \right)}{2 N},

with :math:`\seq{k}{N / 2 - 1}`.

The summation with respect to :math:`n` is split into two parts as well following

.. math::

    \sum_{n = 0}^{N - 1}
    q_n
    =
    &
    \sum_{n = 0}^{N / 2 - 1}
    q_n
    +
    \sum_{n = N / 2}^{N - 1}
    q_n,

    =
    &
    \sum_{n = 0}^{N / 2 - 1}
    q_n
    +
    \sum_{n = 0}^{N / 2 - 1}
    q_{N - 1 - n},

giving

.. math::

    X_{2 k}
    =
    &
    2
    \sum_{n = 0}^{N / 2 - 1}
    x_n
    \ctwiddle{2 \pi}{\left( n + \frac{1}{2} \right) \left( 2 k \right)}{2 N}
    +
    2
    \sum_{n = 0}^{N / 2 - 1}
    x_{N - 1 - n}
    \ctwiddle{2 \pi}{\left( N - 1 - n + \frac{1}{2} \right) \left( 2 k \right)}{2 N},

    X_{2 k + 1}
    =
    &
    2
    \sum_{n = 0}^{N / 2 - 1}
    x_n
    \ctwiddle{2 \pi}{\left( n + \frac{1}{2} \right) \left( 2 k + 1 \right)}{2 N}
    -
    2
    \sum_{n = 0}^{N / 2 - 1}
    x_{N - 1 - n}
    \ctwiddle{2 \pi}{\left( N - 1 - n + \frac{1}{2} \right) \left( 2 k + 1 \right)}{2 N}.

Using :ref:`one of the trigonometric relations derived before <trig_relation_phase>`, we obtain

.. math::

    X_{2 k}
    =
    &
    2
    \sum_{n = 0}^{N / 2 - 1}
    \left(
        x_n
        +
        x_{N - 1 - n}
    \right)
    \ctwiddle{2 \pi}{\left( n + \frac{1}{2} \right) k}{2 \left( N / 2 \right)},

    X_{2 k + 1}
    =
    &
    2
    \sum_{n = 0}^{N / 2 - 1}
    \left(
        x_n
        -
        x_{N - 1 - n}
    \right)
    \ctwiddle{2 \pi}{\left( n + \frac{1}{2} \right) \left( 2 k + 1 \right)}{2 N}.

The first equation is the forward transform having a smaller size:

.. math::

    X_{2 k}
    =
    \dct{N / 2}{k}{x_{0} + x_{N - 1}}{x_{1} + x_{N - 2}}{x_{N / 2 - 1} + x_{N / 2}},

with :math:`\seq{k}{N / 2 - 1}`.

By utilizing :ref:`the identity derived before <trig_relation_prod_sum>` and introducing

.. math::

    \beta \left( n \right)
    \equiv
    2
    \pi
    \frac{
        n + \frac{1}{2}
    }{
        2 N
    },

:math:`X_{2 k + 1}` leads to

.. math::

    &
    \sum_{n = 0}^{N / 2 - 1}
    \frac{
        x_n
        -
        x_{N - 1 - n}
    }{\cos \beta}
    \ctwiddle{2 \pi}{\left( n + \frac{1}{2} \right) \left( k + 1 \right)}{2 \left( N / 2 \right)}
    +
    \sum_{n = 0}^{N / 2 - 1}
    \frac{
        x_n
        -
        x_{N - 1 - n}
    }{\cos \beta}
    \ctwiddle{2 \pi}{\left( n + \frac{1}{2} \right) k}{2 \left( N / 2 \right)}

    =
    &
    \dct{
        N / 2
    }{
        k + 1
    }{
        \frac{
            x_0
            -
            x_{N - 1}
        }{\cos \beta}
    }{
        \frac{
            x_1
            -
            x_{N - 2}
        }{\cos \beta}
    }{
        \frac{
            x_{N / 2 - 1}
            -
            x_{N / 2}
        }{\cos \beta}
    }
    +
    \dct{
        N / 2
    }{
        k
    }{
        \frac{
            x_0
            -
            x_{N - 1}
        }{\cos \beta}
    }{
        \frac{
            x_1
            -
            x_{N - 2}
        }{\cos \beta}
    }{
        \frac{
            x_{N / 2 - 1}
            -
            x_{N / 2}
        }{\cos \beta}
    },

which can be applied directly for :math:`\seq{k}{N / 2 - 2}`.
For :math:`k = N / 2 - 1`, assigning :math:`k = N / 2 - 1` reveals

.. math::

    \sum_{n = 0}^{N / 2 - 1}
    \frac{
        x_n
        -
        x_{N - 1 - n}
    }{\cos \beta}
    \ctwiddle{2 \pi}{\left( n + \frac{1}{2} \right) N / 2}{2 \left( N / 2 \right)}
    =
    \sum_{n = 0}^{N / 2 - 1}
    \frac{
        x_n
        -
        x_{N - 1 - n}
    }{\cos \beta}
    \cos
    \left(
        \frac{2 n + 1}{2}
        \pi
    \right)
    =
    0.

In summary, we obtain the following recurrence relation:

.. math::

    X_{2 k}
    &
    =
    \dct{N / 2}{k}{x_{0} + x_{N - 1}}{x_{1} + x_{N - 2}}{x_{N / 2 - 1} + x_{N / 2}},

    X_{2 k + 1}
    &
    =
    \dct{
        N / 2
    }{
        k + 1
    }{
        \frac{
            x_0
            -
            x_{N - 1}
        }{\cos \beta}
    }{
        \frac{
            x_1
            -
            x_{N - 2}
        }{\cos \beta}
    }{
        \frac{
            x_{N / 2 - 1}
            -
            x_{N / 2}
        }{\cos \beta}
    }

    &
    +
    \dct{
        N / 2
    }{
        k
    }{
        \frac{
            x_0
            -
            x_{N - 1}
        }{\cos \beta}
    }{
        \frac{
            x_1
            -
            x_{N - 2}
        }{\cos \beta}
    }{
        \frac{
            x_{N / 2 - 1}
            -
            x_{N / 2}
        }{\cos \beta}
    },

with :math:`\seq{k}{N / 2 - 1}`.

.. myliteralinclude:: /../../NumericalMethod/FourierTransform/DCT/Lee1984/src/dct.c
    :language: c
    :tag: divide and conquer, forward

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

