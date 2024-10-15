#############
Linear System
#############

Matrix solvers.

.. note::

    Although the functions implemented here behave similarly as the ones in `LAPACK <https://netlib.org/lapack/>`_, there are two major differences:

    * No pivoting

        Although functions in ``LAPACK`` include pivoting operations to stabilize the system, we do not do that for simplicity, assuming all systems are well-conditioned.

    * Treatment of the singularity

        When a singularity is detected, functions in ``LAPACK`` report it and terminate.
        Although this often makes sense, in this project, such exception should be treated gracefully, as singular systems are highly expected due to the imposed Neumann or the periodic boundary conditions.

.. toctree::
    :maxdepth: 1

    gauss_jordan.rst
    ThomasAlgorithm/index.rst

