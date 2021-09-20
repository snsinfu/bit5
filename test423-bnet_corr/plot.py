import matplotlib.pyplot as plt
import numpy as np


def main():
    samples = np.loadtxt("_samples.dat")
    regulation_matrix = np.loadtxt("_regulation_matrix.dat")
    total_correlation = np.loadtxt("_total_correlation.dat")
    direct_correlation = np.loadtxt("_direct_correlation.dat")

    fig = plt.figure()
    grid = plt.GridSpec(2, 3, figure=fig, height_ratios=[1, 3])

    style = {"vmin": -1, "vmax": 1, "cmap": "bwr"}
    fig.add_subplot(grid[0, :]).matshow(samples.T, cmap="gray", aspect="auto")
    fig.add_subplot(grid[1, 0]).matshow(regulation_matrix, **style)
    fig.add_subplot(grid[1, 1]).matshow(total_correlation, **style)
    fig.add_subplot(grid[1, 2]).matshow(direct_correlation, **style)
    plt.show()


main()
