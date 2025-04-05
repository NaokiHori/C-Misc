import numpy as np
from matplotlib import pyplot

nodes = np.loadtxt("nodes.dat")
edges = np.loadtxt("edges.dat")

keywords = {
        "aspect": "equal",
        "xlim": [- 0.5, + 0.5],
        "ylim": [- 0.5, + 0.5],
        }

fig = pyplot.figure()
ax = fig.add_subplot()
for edge in edges:
    _, _, x0, y0, x1, y1 = edge
    ax.plot([x0, x1], [y0, y1], color="#f00")
ax.scatter(nodes[:, 0], nodes[:, 1], color="#000")
ax.set(**keywords)
pyplot.show()
pyplot.close()

