We store :math:`X_k`: the output of the forward transform and the input of the backward transform in the following manner:

.. math::

    x \left[         0 \right] & \leftarrow \real{X_0} = X_0,

    x \left[         1 \right] & \leftarrow \real{X_1},

    & \vdots

    x \left[ N / 2 - 1 \right] & \leftarrow \real{X_{N / 2 - 1}},

    x \left[ N / 2     \right] & \leftarrow \real{X_{N / 2}} = X_{N / 2},

    x \left[ N / 2 + 1 \right] & \leftarrow \imag{X_{N / 2 + 1}},

    x \left[ N / 2 + 2 \right] & \leftarrow \imag{X_{N / 2 + 2}},

    & \vdots

    x \left[ N     - 2 \right] & \leftarrow \imag{X_{N - 2}},

    x \left[ N     - 1 \right] & \leftarrow \imag{X_{N - 1}}.

Namely the real part :math:`\real{X_k}` is stored first up to :math:`k = N / 2`, which is followed by the imaginary part.
Recall the Hermitian symmetry: :math:`\imag{X_k} = - \imag{X_{N - k}}`, and accordingly :math:`\imag{X_0} = \imag{X_{N / 2}} = 0`.

