
.. _tri_diagonal_system:

############
Normal cases
############

We consider a tri-diagonal linear system whose size is :math:`n`:

.. math::

    \begin{bmatrix}
        c_0    & u_0    & 0      & \cdots & 0       & 0       & 0       & \cdots & 0       & 0       & 0       \\
        l_1    & c_1    & u_1    & \cdots & 0       & 0       & 0       & \cdots & 0       & 0       & 0       \\
        0      & l_2    & c_2    & \cdots & 0       & 0       & 0       & \cdots & 0       & 0       & 0       \\
        \vdots & \vdots & \vdots & \ddots & \vdots  & \vdots  & \vdots  &        & \vdots  & \vdots  & \vdots  \\
        0      & 0      & 0      & \cdots & c_{i-1} & u_{i-1} & 0       & \cdots & 0       & 0       & 0       \\
        0      & 0      & 0      & \cdots & l_{i  } & c_{i  } & u_{i  } & \cdots & 0       & 0       & 0       \\
        0      & 0      & 0      & \cdots & 0       & l_{i+1} & c_{i+1} & \cdots & 0       & 0       & 0       \\
        \vdots & \vdots & \vdots &        & \vdots  & \vdots  & \vdots  & \ddots & \vdots  & \vdots  & \vdots  \\
        0      & 0      & 0      & \cdots & 0       & 0       & 0       & \cdots & c_{n-3} & u_{n-3} & 0       \\
        0      & 0      & 0      & \cdots & 0       & 0       & 0       & \cdots & l_{n-2} & c_{n-2} & u_{n-2} \\
        0      & 0      & 0      & \cdots & 0       & 0       & 0       & \cdots & 0       & l_{n-1} & c_{n-1}
    \end{bmatrix}
    \begin{bmatrix}
        x_0     \\
        x_1     \\
        x_2     \\
        \vdots  \\
        x_{i-1} \\
        x_{i  } \\
        x_{i+1} \\
        \vdots  \\
        x_{n-3} \\
        x_{n-2} \\
        x_{n-1}
    \end{bmatrix}
    =
    \begin{bmatrix}
        q_0     \\
        q_1     \\
        q_2     \\
        \vdots  \\
        q_{i-1} \\
        q_{i  } \\
        q_{i+1} \\
        \vdots  \\
        q_{n-3} \\
        q_{n-2} \\
        q_{n-1}
    \end{bmatrix},

where :math:`x_i` is the answer of the system and is to be computed.

For notational simplicity, we concatenate the tri-diagonal matrix and the right-hand-side vector to yield a matrix with :math:`n` rows and :math:`n + 1` columns:

.. math::

    \begin{bmatrix}
        c_0    & u_0    & 0      & \cdots & 0       & 0       & 0       & \cdots & 0       & 0       & 0       & q_0     \\
        l_1    & c_1    & u_1    & \cdots & 0       & 0       & 0       & \cdots & 0       & 0       & 0       & q_1     \\
        0      & l_2    & c_2    & \cdots & 0       & 0       & 0       & \cdots & 0       & 0       & 0       & q_2     \\
        \vdots & \vdots & \vdots & \ddots & \vdots  & \vdots  & \vdots  &        & \vdots  & \vdots  & \vdots  & \vdots  \\
        0      & 0      & 0      & \cdots & c_{i-1} & u_{i-1} & 0       & \cdots & 0       & 0       & 0       & q_{i-1} \\
        0      & 0      & 0      & \cdots & l_{i  } & c_{i  } & u_{i  } & \cdots & 0       & 0       & 0       & q_{i  } \\
        0      & 0      & 0      & \cdots & 0       & l_{i+1} & c_{i+1} & \cdots & 0       & 0       & 0       & q_{i+1} \\
        \vdots & \vdots & \vdots &        & \vdots  & \vdots  & \vdots  & \ddots & \vdots  & \vdots  & \vdots  & \vdots  \\
        0      & 0      & 0      & \cdots & 0       & 0       & 0       & \cdots & c_{n-3} & u_{n-3} & 0       & q_{n-3} \\
        0      & 0      & 0      & \cdots & 0       & 0       & 0       & \cdots & l_{n-2} & c_{n-2} & u_{n-2} & q_{n-2} \\
        0      & 0      & 0      & \cdots & 0       & 0       & 0       & \cdots & 0       & l_{n-1} & c_{n-1} & q_{n-1}
    \end{bmatrix},

and consider `the Gaussian elimination <https://en.wikipedia.org/wiki/Gaussian_elimination>`_.

Our objective is to convert the tri-diagonal matrix to an identity matrix (i.e., matrix inversion):

.. math::

   \begin{bmatrix}
      1      & 0      & 0      & \cdots & 0       & 0       & 0       & \cdots & 0       & 0       & 0       & x_0     \\
      0      & 1      & 0      & \cdots & 0       & 0       & 0       & \cdots & 0       & 0       & 0       & x_1     \\
      0      & 0      & 1      & \cdots & 0       & 0       & 0       & \cdots & 0       & 0       & 0       & x_2     \\
      \vdots & \vdots & \vdots & \ddots & \vdots  & \vdots  & \vdots  &        & \vdots  & \vdots  & \vdots  & \vdots  \\
      0      & 0      & 0      & \cdots & 1       & 0       & 0       & \cdots & 0       & 0       & 0       & x_{i-1} \\
      0      & 0      & 0      & \cdots & 0       & 1       & 0       & \cdots & 0       & 0       & 0       & x_{i  } \\
      0      & 0      & 0      & \cdots & 0       & 0       & 1       & \cdots & 0       & 0       & 0       & x_{i+1} \\
      \vdots & \vdots & \vdots &        & \vdots  & \vdots  & \vdots  & \ddots & \vdots  & \vdots  & \vdots  & \vdots  \\
      0      & 0      & 0      & \cdots & 0       & 0       & 0       & \cdots & 1       & 0       & 0       & x_{n-3} \\
      0      & 0      & 0      & \cdots & 0       & 0       & 0       & \cdots & 0       & 1       & 0       & x_{n-2} \\
      0      & 0      & 0      & \cdots & 0       & 0       & 0       & \cdots & 0       & 0       & 1       & x_{n-1}
   \end{bmatrix}.

=============
Forward Sweep
=============

First we eliminate the lower-diagonal components :math:`l_1, l_2, \cdots, l_{n-2}, l_{n-1}`.
To get started, we look at the top two rows:

.. math::

    \begin{bmatrix}
        c_0 & u_0 & 0   & \cdots & 0 & 0 & 0 & \cdots & 0 & 0 & 0 & q_0 \\
        l_1 & c_1 & u_1 & \cdots & 0 & 0 & 0 & \cdots & 0 & 0 & 0 & q_1
    \end{bmatrix}.

Dividing the first row by :math:`c_0` yields

.. math::

    \begin{bmatrix}
        1   & u_0 / c_0 & 0   & \cdots & 0 & 0 & 0 & \cdots & 0 & 0 & 0 & q_0 / c_0 \\
        l_1 & c_1       & u_1 & \cdots & 0 & 0 & 0 & \cdots & 0 & 0 & 0 & q_1
    \end{bmatrix}
    =
    \begin{bmatrix}
        1   & v_0 & 0   & \cdots & 0 & 0 & 0 & \cdots & 0 & 0 & 0 & r_0 \\
        l_1 & c_1 & u_1 & \cdots & 0 & 0 & 0 & \cdots & 0 & 0 & 0 & q_1
    \end{bmatrix},

namely

.. math::

    v_0 \leftarrow \frac{u_0}{c_0},
    r_0 \leftarrow \frac{q_0}{c_0}.

.. myliteralinclude:: /../../NumericalMethod/LinearSystem/ThomasAlgorithm/normal/src/tri_diagonal_solver.c
    :language: c
    :tag: divide the first row by center-diagonal term

.. note::

    In the code, ``q`` is used to store the solution :math:`x_i` as well as the input :math:`q_i`; namely the input array is overwritten by the solver.

Now we focus on the general :math:`i-1`-th and :math:`i`-th rows:

.. math::

    \begin{bmatrix}
        0 & 0 & 0 & \cdots & 1   & v_{i-1} & 0   & \cdots & 0 & 0 & 0 & r_{i-1} \\
        0 & 0 & 0 & \cdots & l_i & c_i     & u_i & \cdots & 0 & 0 & 0 & q_{i  }
    \end{bmatrix},

where the upper row (:math:`i-1`-th row) has already been updated, while the bottom row (:math:`i`-th row) is to be updated now.

To eliminate :math:`l_i` from the :math:`i`-th row, we subtract the :math:`i-1`-th row times :math:`l_i` from the :math:`i`-th row:

.. math::

    \begin{bmatrix}
        0 & 0 & 0 & \cdots & 1   & v_{i-1}           & 0   & \cdots & 0 & 0 & 0 & r_{i-1} \\
        0 & 0 & 0 & \cdots & 0   & c_i - l_i v_{i-1} & u_i & \cdots & 0 & 0 & 0 & q_{i} - l_i r_{i-1}
    \end{bmatrix}

and divide the :math:`i`-th row by :math:`c_i - l_i v_{i-1}`:

.. math::

    \begin{bmatrix}
        0 & 0 & 0 & \cdots & 1 & v_{i-1} & 0                             & \cdots & 0 & 0 & 0 & r_{i-1} \\
        0 & 0 & 0 & \cdots & 0 & 1       & \frac{u_i}{c_i - l_i v_{i-1}} & \cdots & 0 & 0 & 0 & \frac{q_{i} - l_i r_{i-1}}{c_i - l_i v_{i-1}}
    \end{bmatrix}
    =
    \begin{bmatrix}
        0 & 0 & 0 & \cdots & 1   & v_{i-1} & 0     & \cdots & 0 & 0 & 0 & r_{i-1} \\
        0 & 0 & 0 & \cdots & 0   & 1       & v_{i} & \cdots & 0 & 0 & 0 & r_{i  }
    \end{bmatrix},

where

.. math::

    v_i \leftarrow \frac{u_i}{c_i - l_i v_{i-1}},
    r_i \leftarrow \frac{q_i - l_i r_{i-1}}{c_i - l_i v_{i-1}}.

This is repeated from :math:`i = 1` to :math:`n - 2`:

.. myliteralinclude:: /../../NumericalMethod/LinearSystem/ThomasAlgorithm/normal/src/tri_diagonal_solver.c
    :language: c
    :tag: forward sweep all rows

Basically we do the same thing for the last row, expect the treatment of the singularity; namely the denominator

.. math::

    c_{n-1} - l_{n-1} v_{n-2}

can be :math:`0`, or equivalently the rank of the matrix is :math:`n-1` (degeneracy).
This is expected when dealing with Poisson equations subject to Neumann boundary conditions.

To take into account the singularity and to avoid the resulting zero divisions, we need a special treatment:

.. myliteralinclude:: /../../NumericalMethod/LinearSystem/ThomasAlgorithm/normal/src/tri_diagonal_solver.c
    :language: c
    :tag: forward sweep the last row

=====================
Backward Substitution
=====================

After the forward sweep, we are left with the following system:

.. math::

    \begin{bmatrix}
        1      & v_0    & 0      & \cdots & 0       & 0       & 0      & \cdots & 0       & 0       & 0       \\
        0      & 1      & v_1    & \cdots & 0       & 0       & 0      & \cdots & 0       & 0       & 0       \\
        0      & 0      & 1      & \cdots & 0       & 0       & 0      & \cdots & 0       & 0       & 0       \\
        \vdots & \vdots & \vdots & \ddots & \vdots  & \vdots  & \vdots &        & \vdots  & \vdots  & \vdots  \\
        0      & 0      & 0      & \cdots & 1       & v_{i-1} & 0      & \cdots & 0       & 0       & 0       \\
        0      & 0      & 0      & \cdots & 0       & 1       & v_i    & \cdots & 0       & 0       & 0       \\
        0      & 0      & 0      & \cdots & 0       & 0       & 1      & \cdots & 0       & 0       & 0       \\
        \vdots & \vdots & \vdots &        & \vdots  & \vdots  & \vdots & \ddots & \vdots  & \vdots  & \vdots  \\
        0      & 0      & 0      & \cdots & 0       & 0       & 0      & \cdots & 1       & v_{n-3} & 0       \\
        0      & 0      & 0      & \cdots & 0       & 0       & 0      & \cdots & 0       & 1       & v_{n-2} \\
        0      & 0      & 0      & \cdots & 0       & 0       & 0      & \cdots & 0       & 0       & 1
    \end{bmatrix}
    \begin{bmatrix}
        x_0     \\
        x_1     \\
        x_2     \\
        \vdots  \\
        x_{i-1} \\
        x_{i  } \\
        x_{i+1} \\
        \vdots  \\
        x_{n-3} \\
        x_{n-2} \\
        x_{n-1}
    \end{bmatrix}
    =
    \begin{bmatrix}
        r_0     \\
        r_1     \\
        r_2     \\
        \vdots  \\
        r_{i-1} \\
        r_{i  } \\
        r_{i+1} \\
        \vdots  \\
        r_{n-3} \\
        r_{n-2} \\
        r_{n-1}
    \end{bmatrix}.

The last row:

.. math::

    x_{n-1} = r_{n-1}

has already been computed in the forward sweep and thus we do not have to do anything.
For the rest, since we have

.. math::

    x_i = r_i - v_i x_{i+1},

:math:`x_i` can be computed sequentially from :math:`i = n-2` to :math:`i = 0`, which is known as the backward substitution.

.. myliteralinclude:: /../../NumericalMethod/LinearSystem/ThomasAlgorithm/normal/src/tri_diagonal_solver.c
    :language: c
    :tag: backward substitution

Note again that ``q`` is shared among :math:`x_i` (output) and :math:`q_i` (input) in the code.

.. note::

    Although ``l[0]`` and ``u[n-1]`` are not used in this case, ``l``, ``c``, and ``u`` all have the length :math:`n` for simplicity.
    In particular these values are used for :math:`periodic systems <sherman_morrison>`.

