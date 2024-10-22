A straightforward implementation adopts the recursion.

Forward transform invokes the following kernel function:

.. myliteralinclude:: /../../NumericalMethod/FourierTransform/DFT/src/dft.c
    :language: c
    :tag: main function to perform discrete fourier transform

Backward transform calls the following one:

.. myliteralinclude:: /../../NumericalMethod/FourierTransform/DFT/src/dft.c
    :language: c
    :tag: main function to perform inverse discrete fourier transform

