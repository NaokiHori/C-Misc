import numpy as np
from matplotlib import pyplot

xs = np.loadtxt("xs.dat")
ys = np.loadtxt("ys.dat")
zs = np.loadtxt("zs.dat")
nx = len(xs)
ny = len(ys)
zs = np.reshape(zs, (ny, nx))

line = np.loadtxt("line.dat")
line_xs = line[:, 0]
line_ys = line[:, 1]

fig = pyplot.figure()
ax = fig.add_subplot()
vmin = 0.
vmax = 1.5
levels = np.linspace(vmin, vmax, 1 + int((vmax - vmin) * 10))
print(levels)
ax.contourf(xs, ys, zs, vmin=vmin, vmax=vmax, levels=levels)
ax.plot(line_xs, line_ys, marker="o", color="#fff")
keyword = {
        "aspect": "equal",
        }
ax.set(**keyword)
pyplot.show()
pyplot.close()

