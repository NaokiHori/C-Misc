import json
import numpy
from matplotlib import pyplot, patches

with open("arc.json") as f:
    arc = json.load(f)
x_points = numpy.loadtxt("x_points.dat")
y_points = numpy.loadtxt("y_points.dat")
n_points = [len(x_points), len(y_points)]
distances = numpy.loadtxt("distances.dat")
x_points, y_points = numpy.meshgrid(x_points, y_points)
distances = numpy.reshape(distances, n_points)

figure = pyplot.figure()
axes = figure.add_subplot(111)
levels = numpy.linspace(0., 2.0, 21)
axes.contourf(x_points, y_points, distances, levels=levels)
arc = patches.Arc(
    xy=(arc['center']['x'], arc['center']['y']),
    width=2. * arc['radius'],
    height=2. * arc['radius'],
    theta1=180. / numpy.pi * arc['angles'][0],
    theta2=180. / numpy.pi * arc['angles'][1],
)
axes.add_patch(arc)
axes.set_aspect("equal")
pyplot.show()
pyplot.close()
