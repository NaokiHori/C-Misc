###################
Gauss-Jordan method
###################

We consider a :math:`n \times n`-sized square matrix :math:`A_{ij}`, whose inverse :math:`A_{ij}^{-1}` is computed here (assuming it is a regular matrix).

Starting from the initial state:

.. math::

    \newcommand{\x}[2]{a_{#1,#2}}
    \newcommand{\y}[2]{b_{#1,#2}}
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
        1      & 0      & \cdots & 0      & 0      & 0      & \cdots & 0      & 0      \\
        0      & 1      & \cdots & 0      & 0      & 0      & \cdots & 0      & 0      \\
        \vdots & \vdots & \ddots & \vdots & \vdots & \vdots &        & \vdots & \vdots \\
        0      & 0      & \cdots & 1      & 0      & 0      & \cdots & 0      & 0      \\
        0      & 0      & \cdots & 0      & 1      & 0      & \cdots & 0      & 0      \\
        0      & 0      & \cdots & 0      & 0      & 1      & \cdots & 0      & 0      \\
        \vdots & \vdots &        & \vdots & \vdots & \vdots & \ddots & \vdots & \vdots \\
        0      & 0      & \cdots & 0      & 0      & 0      & \cdots & 1      & 0      \\
        0      & 0      & \cdots & 0      & 0      & 0      & \cdots & 0      & 1      \\
    \end{bmatrix},

we aim at making the first and the second the identity and the inverse matrices, respectively.

*************
Forward sweep
*************

After forward-sweeping up to the :math:`i - 1`-th row, we have

.. math::

    \begin{bmatrix}
        \vdots & \vdots & \ddots & \vdots & \vdots       & \vdots       &        & \vdots       & \vdots       \\
        0      & 0      & \cdots & 1      & \x{i-1}{i  } & \x{i-1}{i+1} & \cdots & \x{i-1}{n-2} & \x{i-1}{n-1} \\
        0      & 0      & \cdots & 0      & \x{i  }{i  } & \x{i  }{i+1} & \cdots & \x{i  }{n-2} & \x{i  }{n-1} \\
        0      & 0      & \cdots & 0      & \x{i+1}{i  } & \x{i+1}{i+1} & \cdots & \x{i+1}{n-2} & \x{i+1}{n-1} \\
        \vdots & \vdots &        & \vdots & \vdots       & \vdots       & \ddots & \vdots       & \vdots       \\
    \end{bmatrix}
    \begin{bmatrix}
        \vdots       & \vdots       & \ddots & \vdots       & \vdots & \vdots &        & \vdots & \vdots \\
        \y{i-1}{  0} & \y{i-1}{  1} & \cdots & \y{i-1}{i-1} & 0      & 0      & \cdots & 0      & 0      \\
        \y{i  }{  0} & \y{i  }{  1} & \cdots & \y{i  }{i-1} & 1      & 0      & \cdots & 0      & 0      \\
        \y{i+1}{  0} & \y{i+1}{  1} & \cdots & \y{i+1}{i-1} & 0      & 1      & \cdots & 0      & 0      \\
        \vdots       & \vdots       &        & \vdots       & \vdots & \vdots & \ddots & \vdots & \vdots \\
    \end{bmatrix}.

The :math:`i`-th row is normalized (divided by :math:`\x{i}{i}`) to obtain

.. math::

    \begin{bmatrix}
        \vdots & \vdots & \ddots & \vdots & \vdots       & \vdots       &        & \vdots       & \vdots       \\
        0      & 0      & \cdots & 1      & \x{i-1}{i  } & \x{i-1}{i+1} & \cdots & \x{i-1}{n-2} & \x{i-1}{n-1} \\
        0      & 0      & \cdots & 0      & 1            & \x{i  }{i+1} & \cdots & \x{i  }{n-2} & \x{i  }{n-1} \\
        0      & 0      & \cdots & 0      & \x{i+1}{i  } & \x{i+1}{i+1} & \cdots & \x{i+1}{n-2} & \x{i+1}{n-1} \\
        \vdots & \vdots &        & \vdots & \vdots       & \vdots       & \ddots & \vdots       & \vdots       \\
    \end{bmatrix}
    \begin{bmatrix}
        \vdots       & \vdots       & \ddots & \vdots       & \vdots       & \vdots &        & \vdots & \vdots \\
        \y{i-1}{  0} & \y{i-1}{  1} & \cdots & \y{i-1}{i-1} & 0            & 0      & \cdots & 0      & 0      \\
        \y{i  }{  0} & \y{i  }{  1} & \cdots & \y{i  }{i-1} & \y{i  }{i  } & 0      & \cdots & 0      & 0      \\
        \y{i+1}{  0} & \y{i+1}{  1} & \cdots & \y{i+1}{i-1} & 0            & 1      & \cdots & 0      & 0      \\
        \vdots       & \vdots       &        & \vdots       & \vdots       & \vdots & \ddots & \vdots & \vdots \\
    \end{bmatrix}.

.. myliteralinclude:: /../../NumericalMethod/LinearSystem/GaussJordan/src/gauss_jordan.c
    :language: c
    :tag: normalize i-th row

The new :math:`i`-th row is utilized to eliminate the :math:`i`-th column:

.. math::

    \begin{bmatrix}
        \vdots & \vdots & \ddots & \vdots & \vdots       & \vdots       &        & \vdots       & \vdots       \\
        0      & 0      & \cdots & 1      & \x{i-1}{i  } & \x{i-1}{i+1} & \cdots & \x{i-1}{n-2} & \x{i-1}{n-1} \\
        0      & 0      & \cdots & 0      & 1            & \x{i  }{i+1} & \cdots & \x{i  }{n-2} & \x{i  }{n-1} \\
        0      & 0      & \cdots & 0      & 0            & \x{i+1}{i+1} & \cdots & \x{i+1}{n-2} & \x{i+1}{n-1} \\
        \vdots & \vdots &        & \vdots & \vdots       & \vdots       & \ddots & \vdots       & \vdots       \\
    \end{bmatrix}
    \begin{bmatrix}
        \vdots       & \vdots       & \ddots & \vdots       & \vdots       & \vdots &        & \vdots & \vdots \\
        \y{i-1}{  0} & \y{i-1}{  1} & \cdots & \y{i-1}{i-1} & 0            & 0      & \cdots & 0      & 0      \\
        \y{i  }{  0} & \y{i  }{  1} & \cdots & \y{i  }{i-1} & \y{i  }{i  } & 0      & \cdots & 0      & 0      \\
        \y{i+1}{  0} & \y{i+1}{  1} & \cdots & \y{i+1}{i-1} & \y{i+1}{i  } & 1      & \cdots & 0      & 0      \\
        \vdots       & \vdots       &        & \vdots       & \vdots       & \vdots & \ddots & \vdots & \vdots \\
    \end{bmatrix}.

.. myliteralinclude:: /../../NumericalMethod/LinearSystem/GaussJordan/src/gauss_jordan.c
    :language: c
    :tag: eliminate lower-triangular part

*********************
Backward substitution
*********************

After the forward sweep, we have

.. math::

    \begin{bmatrix}
        1            & \x{  0}{  1} & \cdots & \x{  0}{i-1} & \x{  0}{i  } & \x{  0}{i+1} & \cdots & \x{  0}{n-2} & \x{  0}{n-1} \\
        0            & 1            & \cdots & \x{  1}{i-1} & \x{  1}{i  } & \x{  1}{i+1} & \cdots & \x{  1}{n-2} & \x{  1}{n-1} \\
        \vdots       & \vdots       & \ddots & \vdots       & \vdots       & \vdots       &        & \vdots       & \vdots       \\
        0            & 0            & \cdots & 1            & \x{i-1}{i  } & \x{i-1}{i+1} & \cdots & \x{i-1}{n-2} & \x{i-1}{n-1} \\
        0            & 0            & \cdots & 0            & 1            & \x{i  }{i+1} & \cdots & \x{i  }{n-2} & \x{i  }{n-1} \\
        0            & 0            & \cdots & 0            & 0            & 1            & \cdots & \x{i+1}{n-2} & \x{i+1}{n-1} \\
        \vdots       & \vdots       &        & \vdots       & \vdots       & \vdots       & \ddots & \vdots       & \vdots       \\
        0            & 0            & \cdots & 0            & 0            & 0            & \cdots & 1            & \x{n-2}{n-1} \\
        0            & 0            & \cdots & 0            & 0            & 0            & \cdots & 0            & 1            \\
    \end{bmatrix}
    \begin{bmatrix}
        \y{  0}{  0} & 0            & \cdots & 0            & 0            & 0            & \cdots & 0            & 0            \\
        \y{  1}{  0} & \y{  1}{  1} & \cdots & 0            & 0            & 0            & \cdots & 0            & 0            \\
        \vdots       & \vdots       & \ddots & \vdots       & \vdots       & \vdots       &        & \vdots       & \vdots       \\
        \y{i-1}{  0} & \y{i-1}{  1} & \cdots & \y{i-1}{i-1} & 0            & 0            & \cdots & 0            & 0            \\
        \y{i  }{  0} & \y{i  }{  1} & \cdots & \y{i  }{i-1} & \y{i  }{i  } & 0            & \cdots & 0            & 0            \\
        \y{i+1}{  0} & \y{i+1}{  1} & \cdots & \y{i+1}{i-1} & \y{i+1}{i  } & \y{i+1}{i+1} & \cdots & 0            & 0            \\
        \vdots       & \vdots       &        & \vdots       & \vdots       & \vdots       & \ddots & \vdots       & \vdots       \\
        \y{n-2}{  0} & \y{n-2}{  1} & \cdots & \y{n-2}{i-1} & \y{n-2}{i  } & \y{n-2}{i+1} & \cdots & \y{n-2}{n-2} & 0            \\
        \y{n-1}{  0} & \y{n-1}{  1} & \cdots & \y{n-1}{i-1} & \y{n-1}{i  } & \y{n-1}{i+1} & \cdots & \y{n-1}{n-2} & \y{n-1}{n-1} \\
    \end{bmatrix}.

Starting from the bottom (:math:`n - 1`-th) row, the upper-triangular elements are eliminated:

.. myliteralinclude:: /../../NumericalMethod/LinearSystem/GaussJordan/src/gauss_jordan.c
    :language: c
    :tag: eliminate upper-triangular part

