######################################
Real-valued Discrete Fourier Transform
######################################

***********
Formulation
***********

=================
Forward transform
=================

.. include:: formulation/forward.rst

==================
Backward transform
==================

.. include:: formulation/backward.rst

**************
Implementation
**************

We store :math:`X_k`: the output of the forward transform and the input of the backward transform in the following order:

.. math::

    x \left[ 0 \right] & \leftarrow \Re \left( X_0 \right) = X_0,

    x \left[ 1 \right] & \leftarrow \Re \left( X_1 \right),

    & \vdots

    x \left[ N / 2 - 1 \right] & \leftarrow \Re \left( X_{N / 2 - 1} \right),

    x \left[ N / 2     \right] & \leftarrow \Re \left( X_{N / 2} \right) = X_{N / 2},

    x \left[ N / 2 + 1 \right] & \leftarrow \Im \left( X_{N / 2} - 1 \right),

    x \left[ N / 2 + 2 \right] & \leftarrow \Im \left( X_{N / 2} - 2 \right),

    & \vdots

    x \left[ N - 2 \right] & \leftarrow \Im \left( X_2 \right),

    x \left[ N - 1 \right] & \leftarrow \Im \left( X_1 \right).

Namely the real part :math:`\Re \left( X_k \right)` is stored in the ascending order first: :math:`k = 0, 1, \cdots, N / 2 - 1, N / 2`, which is followed by the imaginary part :math:`\Im \left( X_k \right)` in the descending order: :math:`k = N / 2 - 1, N / 2 - 2, \cdots, 2, 1`.
Note that :math:`\Im \left( X_0 \right)` and :math:`\Im \left( X_{N / 2} \right)` are zero and thus omitted.
The reason to store the signal as this way is twofold:

* To reuse the input buffer of :math:`x_n` as the output buffer of :math:`X_k`.

* To imply the transforms :math:`\mathcal{F}_N` and :math:`\mathcal{F}_N^{-1}` are :math:`\mathbb{R}^N \rightarrow \mathbb{R}^N`.

=================
Forward transform
=================

.. include:: implementation/forward.rst

==================
Backward transform
==================

.. include:: implementation/backward.rst

*********
Reference
*********

.. include:: reference.rst

