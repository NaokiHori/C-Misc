#################
Forward transform
#################

The `DCT` is defined as

.. math::

    \defdct.

Hereafter, in this page, :math:`X_k` is used instead of :math:`\hat{X}_k` for notational simplicity.

Assuming that :math:`N` is a multiple of :math:`2`, we consider the even and odd wavenumbers separately:

.. math::

    X_{2 k}
    =
    &
    2
    \sum_{n = 0}^{N - 1}
    x_n
    \twiddle{\left( 2 n + 1 \right) \left( 2 k \right)}{2 N},

    X_{2 k + 1}
    =
    &
    2
    \sum_{n = 0}^{N - 1}
    x_n
    \twiddle{\left( 2 n + 1 \right) \left( 2 k + 1 \right)}{2 N},

with :math:`\seq{k}{N / 2 - 1}`.

To make the problem size smaller, the summation with respect to :math:`n` is split into two parts:

.. math::

    \sum_{n = 0}^{N - 1}
    q_n
    =
    &
    \sum_{n = 0}^{N / 2 - 1}
    q_n
    +
    \sum_{n = N / 2}^{N - 1}
    q_n

    =
    &
    \sum_{n = 0}^{N / 2 - 1}
    q_n
    +
    \sum_{n = 0}^{N / 2 - 1}
    q_{N - 1 - n},

with :math:`\seq{k}{N / 2 - 1}`.

Using :ref:`one of the trigonometric relations derived before <trig_relation_phase>`, we obtain

.. math::

    X_{2 k}
    =
    &
    2
    \sum_{n = 0}^{N / 2 - 1}
    x_n
    \twiddle{\left( 2 n + 1 \right) \left( 2 k \right)}{2 N}
    +
    2
    \sum_{n = 0}^{N / 2 - 1}
    x_{N - 1 - n}
    \twiddle{\left( 2 n + 1 \right) \left( 2 k \right)}{2 N}

    =
    &
    2
    \sum_{n = 0}^{N / 2 - 1}
    \left(
        x_n
        +
        x_{N - 1 - n}
    \right)
    \twiddle{\left( 2 n + 1 \right) k}{2 \left( N / 2 \right)},

    X_{2 k + 1}
    =
    &
    2
    \sum_{n = 0}^{N / 2 - 1}
    x_n
    \twiddle{\left( 2 n + 1 \right) \left( 2 k + 1 \right)}{2 N}
    -
    2
    \sum_{n = 0}^{N / 2 - 1}
    x_{N - 1 - n}
    \twiddle{\left( 2 n + 1 \right) \left( 2 k + 1 \right)}{2 N}

    =
    &
    2
    \sum_{n = 0}^{N / 2 - 1}
    \left(
        x_n
        -
        x_{N - 1 - n}
    \right)
    \twiddle{\left( 2 n + 1 \right) \left( 2 k + 1 \right)}{2 N},

where

.. math::

    \beta
    \equiv
    \frac{
        \pi \left( 2 n + 1 \right)
    }{
        2 N
    }.

From the first equation, we simply obtain

.. math::

    X_{2 k}
    =
    \dct{N / 2}{k}{x_{0} + x_{N - 1}}{x_{1} + x_{N - 2}}{x_{N / 2 - 1} + x_{N / 2}},

with :math:`\seq{k}{N / 2 - 1}`.

By utilizing :ref:`the identity derived before <trig_relation_prod_sum>`, the second equation :math:`X_{2 k + 1} = \cdots` is rewritten as

.. math::

    &
    \sum_{n = 0}^{N / 2 - 1}
    \frac{
        x_n
        -
        x_{N - 1 - n}
    }{\cos \beta}
    \twiddle{\left( 2 n + 1 \right) \left( k + 1 \right)}{2 \left( N / 2 \right)}
    +
    \sum_{n = 0}^{N / 2 - 1}
    \frac{
        x_n
        -
        x_{N - 1 - n}
    }{\cos \beta}
    \twiddle{\left( 2 n + 1 \right) k}{2 \left( N / 2 \right)}

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
    }.

For :math:`k = N / 2 - 1`, the first term refers to the :math:`N / 2`-th element.
Although this is undefined, assigning :math:`k = N / 2 - 1` directly to the term reveals

.. math::

    \sum_{n = 0}^{N / 2 - 1}
    \frac{
        x_n
        -
        x_{N - 1 - n}
    }{\cos \beta}
    \twiddle{\left( 2 n + 1 \right) N / 2}{2 \left( N / 2 \right)}
    &
    =
    \sum_{n = 0}^{N / 2 - 1}
    \frac{
        x_n
        -
        x_{N - 1 - n}
    }{\cos \beta}
    \cos
    \left[
        \frac{\pi}{2}
        \left( 2 n + 1 \right)
    \right]

    &
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

with :math:`\seq{k}{N / 2 - 1}` and

.. math::

    \beta
    \equiv
    \frac{
        \pi \left( 2 n + 1 \right)
    }{
        2 N
    }.

