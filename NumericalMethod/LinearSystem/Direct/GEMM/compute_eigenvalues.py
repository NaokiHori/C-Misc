import numpy

matrix = numpy.load('symmetric_operator.npy')
eigenvalues, eigenvectors = numpy.linalg.eigh(matrix)

numpy.save('eigenvalues.npy', eigenvalues)
numpy.save('eigenvectors.npy', eigenvectors)
