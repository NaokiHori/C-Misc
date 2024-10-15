####################
Gaussian elimination
####################

We consider a linear system of size :math:`n`:

.. math::

    A_{ij} x_j = b_i,

or explicitly

.. math::

    \newcommand{\x}[2]{a_{#1,#2}}
    \newcommand{\y}[1]{x_{#1}}
    \newcommand{\z}[1]{b_{#1}}
    \begin{bmatrix}
        \x{  0}{  0} & \x{  0}{  1} & \cdots & \x{  0}{i-1} & \x{  0}{i  } & \x{  0}{i+1} & \cdots & \x{  0}{n-2} & \x{  0}{n-1} \\
        \x{  1}{  0} & \x{  1}{  1} & \cdots & \x{  1}{i-1} & \x{  1}{i  } & \x{  1}{i+1} & \cdots & \x{  1}{n-2} & \x{  1}{n-1} \\
        \vdots       & \vdots       & \ddots & \vdots       & \vdots       & \vdots       &        & \vdots       & \vdots       \\
        \x{i-1}{  0} & \x{i-1}{  1} & \cdots & \x{i-1}{i-1} & \x{i-1}{i  } & \x{i-1}{i+1} & \cdots & \x{i-1}{n-2} & \x{i-1}{n-1} \\
        \x{i  }{  0} & \x{i  }{  1} & \cdots & \x{i  }{i-1} & \x{i  }{i  } & \x{i  }{i+1} & \cdots & \x{i  }{n-2} & \x{i  }{n-1} \\
        \x{i+1}{  0} & \x{i+1}{  1} & \cdots & \x{i+1}{i-1} & \x{i+1}{i  } & \x{i+1}{i+1} & \cdots & \x{i+1}{n-2} & \x{i+1}{n-1} \\
        \vdots       & \vdots       &        & \vdots       & \vdots       & \vdots       & \ddots & \vdots       & \vdots       \\
        \x{n-2}{  0} & \x{n-2}{  1} & \cdots & \x{n-2}{i-1} & \x{n-2}{i  } & \x{n-2}{i+1} & \cdots & \x{n-2}{n-2} & \x{n-2}{n-1} \\
        \x{n-1}{  0} & \x{n-1}{  1} & \cdots & \x{n-1}{i-1} & \x{n-1}{i  } & \x{n-1}{i+1} & \cdots & \x{n-1}{n-2} & \x{n-1}{n-1} \\
    \end{bmatrix}
    \begin{bmatrix}
        \y{  0} \\
        \y{  1} \\
        \vdots  \\
        \y{i-1} \\
        \y{i  } \\
        \y{i+1} \\
        \vdots  \\
        \y{n-2} \\
        \y{n-1} \\
    \end{bmatrix}
    =
    \begin{bmatrix}
        \z{  0} \\
        \z{  1} \\
        \vdots  \\
        \z{i-1} \\
        \z{i  } \\
        \z{i+1} \\
        \vdots  \\
        \z{n-2} \\
        \z{n-1} \\
    \end{bmatrix}.

For notational simplicity, we consider the following augmented matrix:

.. math::

    \begin{bmatrix}
        \x{  0}{  0} & \x{  0}{  1} & \cdots & \x{  0}{i-1} & \x{  0}{i  } & \x{  0}{i+1} & \cdots & \x{  0}{n-2} & \x{  0}{n-1} & \z{  0} \\
        \x{  1}{  0} & \x{  1}{  1} & \cdots & \x{  1}{i-1} & \x{  1}{i  } & \x{  1}{i+1} & \cdots & \x{  1}{n-2} & \x{  1}{n-1} & \z{  1} \\
        \vdots       & \vdots       & \ddots & \vdots       & \vdots       & \vdots       &        & \vdots       & \vdots       & \vdots  \\
        \x{i-1}{  0} & \x{i-1}{  1} & \cdots & \x{i-1}{i-1} & \x{i-1}{i  } & \x{i-1}{i+1} & \cdots & \x{i-1}{n-2} & \x{i-1}{n-1} & \z{i-1} \\
        \x{i  }{  0} & \x{i  }{  1} & \cdots & \x{i  }{i-1} & \x{i  }{i  } & \x{i  }{i+1} & \cdots & \x{i  }{n-2} & \x{i  }{n-1} & \z{i  } \\
        \x{i+1}{  0} & \x{i+1}{  1} & \cdots & \x{i+1}{i-1} & \x{i+1}{i  } & \x{i+1}{i+1} & \cdots & \x{i+1}{n-2} & \x{i+1}{n-1} & \z{i+1} \\
        \vdots       & \vdots       &        & \vdots       & \vdots       & \vdots       & \ddots & \vdots       & \vdots       & \vdots  \\
        \x{n-2}{  0} & \x{n-2}{  1} & \cdots & \x{n-2}{i-1} & \x{n-2}{i  } & \x{n-2}{i+1} & \cdots & \x{n-2}{n-2} & \x{n-2}{n-1} & \z{n-2} \\
        \x{n-1}{  0} & \x{n-1}{  1} & \cdots & \x{n-1}{i-1} & \x{n-1}{i  } & \x{n-1}{i+1} & \cdots & \x{n-1}{n-2} & \x{n-1}{n-1} & \z{n-1} \\
    \end{bmatrix}.

*************
Forward sweep
*************

After forward-sweeping up to the :math:`i - 1`-th row, we have

.. include:: math/1.txt

The :math:`i`-th row is normalized (divided by :math:`\x{i}{i}`) to obtain

.. include:: math/2.txt

.. myliteralinclude:: /../../NumericalMethod/LinearSystem/GaussianElimination/src/gaussian_elimination.c
    :language: c
    :tag: normalize i-th row

The new :math:`i`-th row is utilized to eliminate the :math:`i`-th column:

.. include:: math/3.txt

.. myliteralinclude:: /../../NumericalMethod/LinearSystem/GaussianElimination/src/gaussian_elimination.c
    :language: c
    :tag: eliminate lower-triangular part

*********************
Backward substitution
*********************

After the forward sweep, we have

.. math::

    \begin{bmatrix}
        1            & \x{  0}{  1} & \cdots & \x{  0}{i-1} & \x{  0}{i  } & \x{  0}{i+1} & \cdots & \x{  0}{n-2} & \x{  0}{n-1} & \z{  0} \\
        0            & 1            & \cdots & \x{  1}{i-1} & \x{  1}{i  } & \x{  1}{i+1} & \cdots & \x{  1}{n-2} & \x{  1}{n-1} & \z{  1} \\
        \vdots       & \vdots       & \ddots & \vdots       & \vdots       & \vdots       &        & \vdots       & \vdots       & \vdots  \\
        0            & 0            & \cdots & 1            & \x{i-1}{i  } & \x{i-1}{i+1} & \cdots & \x{i-1}{n-2} & \x{i-1}{n-1} & \z{i-1} \\
        0            & 0            & \cdots & 0            & 1            & \x{i  }{i+1} & \cdots & \x{i  }{n-2} & \x{i  }{n-1} & \z{i  } \\
        0            & 0            & \cdots & 0            & 0            & 1            & \cdots & \x{i+1}{n-2} & \x{i+1}{n-1} & \z{i+1} \\
        \vdots       & \vdots       &        & \vdots       & \vdots       & \vdots       & \ddots & \vdots       & \vdots       & \vdots  \\
        0            & 0            & \cdots & 0            & 0            & 0            & \cdots & 1            & \x{n-2}{n-1} & \z{n-2} \\
        0            & 0            & \cdots & 0            & 0            & 0            & \cdots & 0            & 1            & \y{n-1} \\
    \end{bmatrix}.

Starting from the bottom (:math:`n - 1`-th) row, the upper-triangular elements are eliminated.

After backward-substituting up to the :math:`i + 1`-th row, we have

.. math::

    \begin{bmatrix}
        \vdots       & \vdots       & \ddots & \vdots       & \vdots       & \vdots       &        & \vdots       & \vdots       & \vdots  \\
        0            & 0            & \cdots & 1            & \x{i-1}{i  } & \x{i-1}{i+1} & \cdots & \x{i-1}{n-2} & \x{i-1}{n-1} & \z{i-1} \\
        0            & 0            & \cdots & 0            & 1            & \x{i  }{i+1} & \cdots & \x{i  }{n-2} & \x{i  }{n-1} & \z{i  } \\
        0            & 0            & \cdots & 0            & 0            & 1            & \cdots & \x{i+1}{n-2} & \x{i+1}{n-1} & \y{i+1} \\
        \vdots       & \vdots       &        & \vdots       & \vdots       & \vdots       & \ddots & \vdots       & \vdots       & \vdots  \\
    \end{bmatrix}.

.. myliteralinclude:: /../../NumericalMethod/LinearSystem/GaussianElimination/src/gaussian_elimination.c
    :language: c
    :tag: backward substitution

