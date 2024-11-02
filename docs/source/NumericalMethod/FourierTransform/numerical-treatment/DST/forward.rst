We consider :ref:`the forward transform (discrete sine transform of type II) <discrete_sine_transform>`:

.. math::

    X_k
    \equiv
    2
    \sum_{n = 0}^{N - 1}
    x_n
    \stwiddle{
        2 \pi
    }{
        \left( n + \frac{1}{2} \right)
        \left( k + 1 \right)
    }{
        2 N
    }
    \equiv
    \dstii{k}{N}{x_0}{x_1}{x_{N - 1}}

for :math:`\seq{k}{0}{1}{N - 1}`.

Assuming that :math:`N` is a multiple of :math:`2`, we consider the even and odd wavenumbers separately:

.. math::

    X_{2 k}
    =
    &
    2
    \sum_{n = 0}^{N - 1}
    x_n
    \stwiddle{2 \pi}{\left( n + \frac{1}{2} \right) \left( 2 k + 1 \right)}{2 N},

    X_{2 k + 1}
    =
    &
    2
    \sum_{n = 0}^{N - 1}
    x_n
    \stwiddle{2 \pi}{\left( n + \frac{1}{2} \right) \left( 2 k + 2 \right)}{2 N},

for :math:`\seq{k}{0}{1}{N / 2 - 1}`.

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
    \stwiddle{2 \pi}{\left( n + \frac{1}{2} \right) \left( 2 k + 1 \right)}{2 N}
    +
    2
    \sum_{n = 0}^{N / 2 - 1}
    x_{N - 1 - n}
    \stwiddle{2 \pi}{\left( N - 1 - n + \frac{1}{2} \right) \left( 2 k + 1 \right)}{2 N}

    =
    &
    2
    \sum_{n = 0}^{N / 2 - 1}
    x_n
    \stwiddle{2 \pi}{\left( n + \frac{1}{2} \right) \left( 2 k + 1 \right)}{2 N}
    +
    2
    \sum_{n = 0}^{N / 2 - 1}
    x_{N - 1 - n}
    \stwiddle{2 \pi}{\left( n + \frac{1}{2} \right) \left( 2 k + 1 \right)}{2 N}

    =
    &
    2
    \sum_{n = 0}^{N / 2 - 1}
    \left(
        x_n
        +
        x_{N - 1 - n}
    \right)
    \stwiddle{2 \pi}{\left( n + \frac{1}{2} \right) \left( 2 k + 1 \right)}{2 N},

and

.. math::

    X_{2 k + 1}
    =
    &
    2
    \sum_{n = 0}^{N / 2 - 1}
    x_n
    \stwiddle{2 \pi}{\left( n + \frac{1}{2} \right) \left( 2 k + 2 \right)}{2 N}
    +
    2
    \sum_{n = 0}^{N / 2 - 1}
    x_{N - 1 - n}
    \stwiddle{2 \pi}{\left( N - 1 - n + \frac{1}{2} \right) \left( 2 k + 2 \right)}{2 N}

    =
    &
    2
    \sum_{n = 0}^{N / 2 - 1}
    x_n
    \stwiddle{2 \pi}{\left( n + \frac{1}{2} \right) \left( 2 k + 2 \right)}{2 N}
    -
    2
    \sum_{n = 0}^{N / 2 - 1}
    x_{N - 1 - n}
    \stwiddle{2 \pi}{\left( n + \frac{1}{2} \right) \left( 2 k + 2 \right)}{2 N}

    =
    &
    2
    \sum_{n = 0}^{N / 2 - 1}
    \left(
        x_n
        -
        x_{N - 1 - n}
    \right)
    \stwiddle{2 \pi}{\left( n + \frac{1}{2} \right) \left( k + 1 \right)}{2 \left( N / 2 \right)}

    =
    &
    \dstii{k}{N / 2}{x_0 - x_{N - 1}}{x_1 - x_{N - 2}}{x_{N / 2 - 1} - x_{N / 2}}

for :math:`\seq{k}{0}{1}{N / 2 - 1}`.
Note that we adopt :ref:`the trigonometric relations derived before <dst_trigonometric_relation>`.

By utilizing the product-to-sum identity:

.. math::

    \sin \alpha
    \equiv
    \frac{
        \sin \left( \alpha + \beta \right)
        +
        \sin \left( \alpha - \beta \right)
    }{
        2 \cos \beta
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

we obtain

.. math::

    X_{2 k}
    =
    &
    2
    \sum_{n = 0}^{N / 2 - 1}
    \frac{
        x_n
        +
        x_{N - 1 - n}
    }{
        2 \cos \beta
    }
    \stwiddle{2 \pi}{\left( n + \frac{1}{2} \right) k}{2 \left( N / 2 \right)}
    +
    2
    \sum_{n = 0}^{N / 2 - 1}
    \frac{
        x_n
        +
        x_{N - 1 - n}
    }{
        2 \cos \beta
    }
    \stwiddle{2 \pi}{\left( n + \frac{1}{2} \right) \left( k + 1 \right)}{2 \left( N / 2 \right)}

    =
    &
    \dstii{
        k - 1
    }{
        N / 2
    }{
        \frac{
            x_0
            +
            x_{N - 1}
        }{
            2 \cos \beta
        }
    }{
        \frac{
            x_1
            +
            x_{N - 2}
        }{
            2 \cos \beta
        }
    }{
        \frac{
            x_{N / 2 - 1}
            +
            x_{N / 2}
        }{
            2 \cos \beta
        }
    }
    +
    \dstii{
        k
    }{
        N / 2
    }{
        \frac{
            x_0
            +
            x_{N - 1}
        }{
            2 \cos \beta
        }
    }{
        \frac{
            x_1
            +
            x_{N - 2}
        }{
            2 \cos \beta
        }
    }{
        \frac{
            x_{N / 2 - 1}
            +
            x_{N / 2}
        }{
            2 \cos \beta
        }
    },

which can be applied directly for :math:`\seq{k}{1}{2}{N / 2 - 1}`.

For :math:`k = 0` (i.e. a corner case), assigning :math:`k = 0` to the first term reveals

.. math::

    \dstii{
        - 1
    }{
        N / 2
    }{
        \frac{
            x_0
            +
            x_{N - 1}
        }{
            2 \cos \beta
        }
    }{
        \frac{
            x_1
            +
            x_{N - 2}
        }{
            2 \cos \beta
        }
    }{
        \frac{
            x_{N / 2 - 1}
            +
            x_{N / 2}
        }{
            2 \cos \beta
        }
    }
    =
    0.

In summary, we obtain the following recurrence relation:

.. math::

    X_{2 k}
    &
    =
    \dstii{
        k - 1
    }{
        N / 2
    }{
        \frac{
            x_0
            +
            x_{N - 1}
        }{
            2 \cos \beta
        }
    }{
        \frac{
            x_1
            +
            x_{N - 2}
        }{
            2 \cos \beta
        }
    }{
        \frac{
            x_{N / 2 - 1}
            +
            x_{N / 2}
        }{
            2 \cos \beta
        }
    }

    &
    +
    \dstii{
        k
    }{
        N / 2
    }{
        \frac{
            x_0
            +
            x_{N - 1}
        }{
            2 \cos \beta
        }
    }{
        \frac{
            x_1
            +
            x_{N - 2}
        }{
            2 \cos \beta
        }
    }{
        \frac{
            x_{N / 2 - 1}
            +
            x_{N / 2}
        }{
            2 \cos \beta
        }
    },

    X_{2 k + 1}
    &
    =
    \dstii{
        k
    }{
        N / 2
    }{
        x_{0} - x_{N - 1}
    }{
        x_{1} - x_{N - 2}
    }{
        x_{N / 2 - 1} - x_{N / 2}
    },

which is valid for :math:`\seq{k}{0}{1}{N / 2 - 1}`.
Recall that

.. math::

    \dstii{
        - 1
    }{
        N / 2
    }{
        \frac{
            x_0
            +
            x_{N - 1}
        }{
            2 \cos \beta
        }
    }{
        \frac{
            x_1
            +
            x_{N - 2}
        }{
            2 \cos \beta
        }
    }{
        \frac{
            x_{N / 2 - 1}
            +
            x_{N / 2}
        }{
            2 \cos \beta
        }
    }
    =
    0.

Note that, :math:`X_0 = 2 x_0` is satisfied for :math:`N = 1`.

.. myliteralinclude:: /../../NumericalMethod/FourierTransform/DST/src/dst.c
    :language: c
    :tag: forward transform

