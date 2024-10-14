
.. _sherman_morrison:

###############
Perturbed cases
###############

We consider the following system:

.. math::

    \begin{bmatrix}
        c_0     & u_0    & 0      & \cdots & 0       & 0       & 0       & \cdots & 0       & 0       & l_0     \\
        l_1     & c_1    & u_1    & \cdots & 0       & 0       & 0       & \cdots & 0       & 0       & 0       \\
        0       & l_2    & c_2    & \cdots & 0       & 0       & 0       & \cdots & 0       & 0       & 0       \\
        \vdots  & \vdots & \vdots & \ddots & \vdots  & \vdots  & \vdots  &        & \vdots  & \vdots  & \vdots  \\
        0       & 0      & 0      & \cdots & c_{i-1} & u_{i-1} & 0       & \cdots & 0       & 0       & 0       \\
        0       & 0      & 0      & \cdots & l_{i  } & c_{i  } & u_{i  } & \cdots & 0       & 0       & 0       \\
        0       & 0      & 0      & \cdots & 0       & l_{i+1} & c_{i+1} & \cdots & 0       & 0       & 0       \\
        \vdots  & \vdots & \vdots &        & \vdots  & \vdots  & \vdots  & \ddots & \vdots  & \vdots  & \vdots  \\
        0       & 0      & 0      & \cdots & 0       & 0       & 0       & \cdots & c_{n-3} & u_{n-3} & 0       \\
        0       & 0      & 0      & \cdots & 0       & 0       & 0       & \cdots & l_{n-2} & c_{n-2} & u_{n-2} \\
        u_{n-1} & 0      & 0      & \cdots & 0       & 0       & 0       & \cdots & 0       & l_{n-1} & c_{n-1}
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

where, compare with :ref:`the original tri-diagonal system <tri_diagonal_system>`, the top-right and the bottom-left corners have non-zero values (:math:`l_0` and :math:`u_{n - 1}`, respectively), which appears in particular when periodic boundary conditions are imposed.

Since we have

.. math::

    c_0 x_0
    +
    u_0 x_1
    &
    =
    q_0
    -
    l_0 x_{n - 1},

    l_{n - 2} x_{n - 3}
    +
    c_{n - 2} x_{n - 2}
    &
    =
    q_{n - 2}
    -
    u_{n - 2} x_{n - 1},

at the :math:`0`-th and the :math:`n - 2`-th rows, we reformulate the system as

.. math::

    \begin{bmatrix}
        c_0     & u_0    & 0      & \cdots & 0       & 0       & 0       & \cdots & 0       & 0       & 0       \\
        l_1     & c_1    & u_1    & \cdots & 0       & 0       & 0       & \cdots & 0       & 0       & 0       \\
        0       & l_2    & c_2    & \cdots & 0       & 0       & 0       & \cdots & 0       & 0       & 0       \\
        \vdots  & \vdots & \vdots & \ddots & \vdots  & \vdots  & \vdots  &        & \vdots  & \vdots  & \vdots  \\
        0       & 0      & 0      & \cdots & c_{i-1} & u_{i-1} & 0       & \cdots & 0       & 0       & 0       \\
        0       & 0      & 0      & \cdots & l_{i  } & c_{i  } & u_{i  } & \cdots & 0       & 0       & 0       \\
        0       & 0      & 0      & \cdots & 0       & l_{i+1} & c_{i+1} & \cdots & 0       & 0       & 0       \\
        \vdots  & \vdots & \vdots &        & \vdots  & \vdots  & \vdots  & \ddots & \vdots  & \vdots  & \vdots  \\
        0       & 0      & 0      & \cdots & 0       & 0       & 0       & \cdots & c_{n-4} & u_{n-4} & 0       \\
        0       & 0      & 0      & \cdots & 0       & 0       & 0       & \cdots & l_{n-3} & c_{n-3} & u_{n-3} \\
        0       & 0      & 0      & \cdots & 0       & 0       & 0       & \cdots & 0       & l_{n-2} & c_{n-2}
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
        x_{n-4} \\
        x_{n-3} \\
        x_{n-2}
    \end{bmatrix}
    =
    \begin{bmatrix}
        q_0 - l_0 x_{n-1} \\
        q_1               \\
        q_2               \\
        \vdots            \\
        q_{i-1}           \\
        q_{i  }           \\
        q_{i+1}           \\
        \vdots            \\
        q_{n-4}           \\
        q_{n-3}           \\
        q_{n-2} - u_{n-2} x_{n-1}
    \end{bmatrix}.

Note that we kick out :math:`x_{n-1}` from the left-hand side, and the resulting system is now tri-diagonal with the size of :math:`n - 1`.
For notational simplicity, hereafter we write this system as

.. math::

    A_{ij}
    x_j
    =
    b_i.

Although this is a purely tri-diagonal system (i.e., without perturbed elements), we cannot perform forward sweep as an unknown :math:`x_{n - 1}` exists in the right-hand-side vector.
To circumvent this issue, we split the system into two sub problems:

.. math::

    A_{ij}
    x_j^{\prime}
    &
    =
    b_i^{\prime},

    A_{ij}
    x_j^{\prime\prime}
    &
    =
    b_i^{\prime\prime},

where

.. math::

    b_i^{\prime}
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
        q_{n-4} \\
        q_{n-3} \\
        q_{n-2}
    \end{bmatrix},
    b_i^{\prime\prime}
    =
    \begin{bmatrix}
        - l_0  \\
        0      \\
        0      \\
        \vdots \\
        0      \\
        0      \\
        0      \\
        \vdots \\
        0      \\
        0      \\
        - u_{n-2}
    \end{bmatrix},

which satisfy

.. math::

    &
    x_j
    =
    x_j^{\prime}
    +
    x_{n-1}
    x_j^{\prime\prime},

    &
    b_i
    =
    b_i^{\prime}
    +
    x_{n-1}
    b_i^{\prime\prime},

and thus the solution :math:`x_j` is found as the superposition of the solution of these two sub systems.

To find :math:`x_{n - 1}`, we look at the :math:`n - 1`-th row of the original system:

.. math::

    u_{n - 1} x_0
    +
    l_{n - 1} x_{n - 2}
    +
    c_{n - 1} x_{n - 1}
    =
    q_{n - 1}.

By assigning

.. math::

    x_0
    &
    =
    x_0^{\prime}
    +
    x_{n - 1}
    x_0^{\prime\prime},

    x_{n - 2}
    &
    =
    x_{n - 2}^{\prime}
    +
    x_{n - 1}
    x_{n - 2}^{\prime\prime},

we notice

.. math::

    \left( u_{n - 1} x_0^{\prime}       + l_{n - 1} x_{n - 2}^{\prime}                   \right)
    +
    \left( u_{n - 1} x_0^{\prime\prime} + l_{n - 1} x_{n - 2}^{\prime\prime} + c_{n - 1} \right) x_{n - 1}
    =
    q_{n - 1},

and thus

.. math::

    x_{n - 1}
    =
    \frac{
        q_{n - 1} - u_{n - 1} x_0^{\prime}       - l_{n - 1} x_{n - 2}^{\prime}
    }{
        c_{n - 1} + u_{n - 1} x_0^{\prime\prime} + l_{n - 1} x_{n - 2}^{\prime\prime}
    }.

To summarize, the perturbed system is solved as follows.

#. Prepare the right-hand-side vector of a sub system :math:`b_i^{\prime\prime}`:

    .. myliteralinclude:: /../../NumericalMethod/LinearSystem/ThomasAlgorithm/perturbed/src/tri_diagonal_solver.c
        :language: c
        :tag: prepare a sub system

#. Solve two sub systems:

    Forward sweep:

    .. myliteralinclude:: /../../NumericalMethod/LinearSystem/ThomasAlgorithm/perturbed/src/tri_diagonal_solver.c
        :language: c
        :tag: divide the first row by center-diagonal term

    .. myliteralinclude:: /../../NumericalMethod/LinearSystem/ThomasAlgorithm/perturbed/src/tri_diagonal_solver.c
        :language: c
        :tag: forward sweep all rows

    .. myliteralinclude:: /../../NumericalMethod/LinearSystem/ThomasAlgorithm/perturbed/src/tri_diagonal_solver.c
        :language: c
        :tag: forward sweep the last row

    Backward substitution:

    .. myliteralinclude:: /../../NumericalMethod/LinearSystem/ThomasAlgorithm/perturbed/src/tri_diagonal_solver.c
        :language: c
        :tag: backward substitution

#. Couple the result of two sub systems:

    .. myliteralinclude:: /../../NumericalMethod/LinearSystem/ThomasAlgorithm/perturbed/src/tri_diagonal_solver.c
        :language: c
        :tag: couple two systems to find the answer

