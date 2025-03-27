import sympy


def find_constraint(a: sympy.Symbol, b: sympy.Symbol) -> sympy.Basic:
    c = 3 * a - 2
    a10 = a
    a20 = b / a * (3 * a * a - 3 * a + b) / c
    a21 = - b / a * (b - a) / c
    a30 = 1 - (3 * a + 3 * b - 2) / (6 * a * b)
    a31 = (3 * b - 2) / (6 * a * (b - a))
    a32 = - c / (6 * b * (b - a))
    eq = (a20 - a10) * (a31 - a21) - a21 * (a30 - a20)
    return sympy.simplify(eq)


def show_butcher_tableau(a: sympy.Symbol, b: sympy.Symbol):
    b0 = 0
    b1 = a
    b2 = b
    b3 = 0
    a00 = 0
    a01 = 0
    a02 = 0
    a10 = a
    a11 = 0
    a12 = 0
    a20 = b / a * (b - 3 * a * (1 - a)) / (3 * a - 2)
    a21 = - b / a * (b - a) / (3 * a - 2)
    a22 = 0
    a30 = 1 - (3 * a + 3 * b - 2) / (6 * a * b)
    a31 = (3 * b - 2) / (6 * a * (b - a))
    a32 = (2 - 3 * a) / (6 * b * (b - a))
    print(f"{b0} {a00} {a01} {a02}")
    print(f"{b1} {a10} {a11} {a12}")
    print(f"{b2} {a20} {a21} {a22}")
    print(f"{b3} {a30} {a31} {a32}")



def visualize(a: sympy.Symbol, b: sympy.Symbol, eq: sympy.Basic):
    import numpy as np
    import matplotlib
    matplotlib.use("Agg")
    from matplotlib import pyplot
    # solve "numerator = 0"
    eq, _ = sympy.fraction(eq)
    xlim = [-0.25, 1.]
    ylim = [-0.25, 1.]
    num_points = 32
    values = np.zeros((num_points, num_points))
    xs = np.linspace(xlim[0], xlim[1], num_points)
    ys = np.linspace(ylim[0], ylim[1], num_points)
    for i, y in enumerate(ys):
        for j, x in enumerate(xs):
            values[i, j] = eq.subs([(a, x), (b, y)])
    figure = pyplot.figure()
    axes = [
        figure.add_subplot(),
    ]
    axes[0].contour(xs, ys, values, levels=[0], colors="#FF0000")
    keywords = {
        "aspect": "equal",
        "xlim": xlim,
        "ylim": ylim,
        "xlabel": "$\\alpha$",
        "ylabel": "$\\beta$",
        "xticks": [0, 0.5, 1],
        "yticks": [0, 0.5, 1],
    }
    axes[0].set(**keywords)
    pyplot.savefig("third_order_methods_constraint.jpg")
    pyplot.close()


def main():
    a = sympy.Symbol("a")
    b = sympy.Symbol("b")
    eq = find_constraint(a, b)
    print(eq)
    visualize(a, b, eq)


main()

