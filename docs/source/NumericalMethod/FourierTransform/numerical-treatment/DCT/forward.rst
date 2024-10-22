We consider :ref:`the forward transform (discrete cosine transform of type II) <discrete_cosine_transform>`:

.. math::

    X_k
    \equiv
    2
    \sum_{n = 0}^{N - 1}
    x_n
    \ctwiddle{2 \pi}{\left( n + \frac{1}{2} \right) k}{2 N}.

Here we introduce a symbol for brevity:

.. math::

    \dctii{k}{N}{x_0}{x_1}{x_{N - 1}}
    \equiv
    2
    \sum_{n = 0}^{N - 1}
    x_n
    \ctwiddle{2 \pi}{\left( n + \frac{1}{2} \right) k}{2 N}.

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
    \ctwiddle{2 \pi}{\left( N - 1 - n + \frac{1}{2} \right) \left( 2 k \right)}{2 N}

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
    \ctwiddle{2 \pi}{\left( n + \frac{1}{2} \right) \left( 2 k \right)}{2 N}

    =
    &
    2
    \sum_{n = 0}^{N / 2 - 1}
    \left(
        x_n
        +
        x_{N - 1 - n}
    \right)
    \ctwiddle{2 \pi}{\left( n + \frac{1}{2} \right) k}{2 \left( N / 2 \right)}

    =
    &
    \dctii{k}{N / 2}{x_0 + x_{N - 1}}{x_1 + x_{N - 2}}{x_{N / 2 - 1} + x_{N / 2}},

and

.. math::

    X_{2 k + 1}
    =
    &
    2
    \sum_{n = 0}^{N / 2 - 1}
    x_n
    \ctwiddle{2 \pi}{\left( n + \frac{1}{2} \right) \left( 2 k + 1 \right)}{2 N}
    +
    2
    \sum_{n = 0}^{N / 2 - 1}
    x_{N - 1 - n}
    \ctwiddle{2 \pi}{\left( N - 1 - n + \frac{1}{2} \right) \left( 2 k + 1 \right)}{2 N}

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
    \ctwiddle{2 \pi}{\left( n + \frac{1}{2} \right) \left( 2 k + 1 \right)}{2 N}

    =
    &
    2
    \sum_{n = 0}^{N / 2 - 1}
    \left(
        x_n
        -
        x_{N - 1 - n}
    \right)
    \ctwiddle{2 \pi}{\left( n + \frac{1}{2} \right) \left( 2 k + 1 \right)}{2 N},

with :math:`\seq{k}{N / 2 - 1}`.
Note that we adopt :ref:`the trigonometric relations derived before <trigonometric_relation>`.

By utilizing the product-to-sum identity:

.. math::

    2 \cos \alpha
    \equiv
    \frac{
        \cos \left( \alpha + \beta \right)
        +
        \cos \left( \alpha - \beta \right)
    }{
        \cos \beta
    }

with

.. math::

    \beta
    =
    2 \pi
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
    \dctii{
        k + 1
    }{
        N / 2
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
    \dctii{
        k
    }{
        N / 2
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
For :math:`k = N / 2 - 1` (i.e. a corner case), assigning :math:`k = N / 2 - 1` to the first term reveals

.. math::

    \mathcal{C}_{N / 2}
    &
    =
    \sum_{n = 0}^{N / 2 - 1}
    \frac{
        x_n
        -
        x_{N - 1 - n}
    }{\cos \beta}
    \ctwiddle{2 \pi}{\left( n + \frac{1}{2} \right) N / 2}{2 \left( N / 2 \right)}

    &
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

    &
    =
    0.

In summary, we obtain the following recurrence relation:

.. math::

    X_{2 k}
    &
    =
    \dctii{k}{N / 2}{x_{0} + x_{N - 1}}{x_{1} + x_{N - 2}}{x_{N / 2 - 1} + x_{N / 2}},

    X_{2 k + 1}
    &
    =
    \dctii{
        k
    }{
        N / 2
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
    \dctii{
        k + 1
    }{
        N / 2
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

which is valid for :math:`\seq{k}{N / 2 - 1}`.
Recall that :math:`\mathcal{C}_{N / 2} \equiv 0`.
Note that, :math:`X_0 = 2 x_0` for :math:`N = 1`.

.. myliteralinclude:: /../../NumericalMethod/FourierTransform/DCT/Lee1984/src/dct.c
    :language: c
    :tag: forward transform

