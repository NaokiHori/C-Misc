import numpy as np
from matplotlib import pyplot

def load_blocks_from_file(filename):
    blocks = []
    current_block = []
    with open(filename, "r") as file:
        for line in file:
            line = line.strip()
            if line:
                current_block.append(list(map(float, line.split())))
            else:
                if current_block:
                    blocks.append(np.array(current_block))
                    current_block = []
    if current_block:
        blocks.append(np.array(current_block))
    return blocks

vectors = load_blocks_from_file("vectors.dat")
potential = np.load("potential.npy")

lx = 2. * np.pi
ly = 2. * np.pi

ny, nx = potential.shape

x = lx * (np.arange(nx) + 0.5) / nx
y = ly * (np.arange(ny) + 0.5) / ny

fig = pyplot.figure()
axes = [
    fig.add_subplot(121),
    fig.add_subplot(122),
]
axes[0].contourf(x, y, potential, cmap="gray", levels=101)
for vector in vectors:
    axes[0].plot(vector[:, 1], vector[:, 2], color="#00FF00")
    axes[0].scatter([vector[-1, 1]], [vector[-1, 2]], color="#0000FF")
    axes[1].plot(vector[:, 0], vector[:, 3])
axes[0].set_xlim([0., lx])
axes[0].set_ylim([0., ly])
axes[0].set_aspect("equal")
axes[1].set_yscale("log")
pyplot.show()
pyplot.close()

