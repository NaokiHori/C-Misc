A straightforward implementation adopts the recursion:

.. myliteralinclude:: /../../NumericalMethod/FourierTransform/DFT/src/dft.c
    :language: c
    :tag: divide-and-conquer

The answer of the sub problems are combined to be the solution of the original bigger problem:

.. myliteralinclude:: /../../NumericalMethod/FourierTransform/DFT/src/dft.c
    :language: c
    :tag: unify two sub problem results

Note that the difference between the forward and the inverse transforms is absorbed into a single variable ``sign``, which takes :math:`-1` and :math:`+1`, respectively.

