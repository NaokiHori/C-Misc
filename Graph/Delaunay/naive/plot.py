import numpy as np
from matplotlib import pyplot
from tqdm import tqdm

nodes = np.loadtxt("nodes.dat")
edges = np.loadtxt("edges.dat", dtype=np.uint64)

color = "#000"
keywords = {
        "aspect": "equal",
        "xlim": [- 0.5, + 0.5],
        "ylim": [- 0.5, + 0.5],
        }

fig = pyplot.figure()
ax = fig.add_subplot()
# ax.scatter(nodes[:, 0], nodes[:, 1], color=color)
for edge in tqdm(edges):
    i0 = edge[0]
    i1 = edge[1]
    x0, y0 = nodes[i0]
    x1, y1 = nodes[i1]
    ax.plot([x0, x1], [y0, y1], color=color)
ax.set(**keywords)
pyplot.show()
pyplot.close()

