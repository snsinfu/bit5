import matplotlib.pyplot as plt
import numpy as np


def main():
    dimension = 7
    subst = 0b101010 # Use this to coherently substitute vertex IDs
    distortion = 0 # Increase this to mitigate vertex overlaps by distorting embedding

    assert subst < 2 ** dimension

    angles = np.pi * np.arange(dimension) / dimension
    angles += np.linspace(0, distortion, len(angles))
    basis = np.transpose([np.cos(angles), np.sin(angles)])

    vertex_count = 2 ** dimension
    vertex_ids = np.arange(vertex_count)
    vertex_codes = binary_code(vertex_ids, width=dimension)
    vertex_points = vertex_codes @ basis
    edges = hypercube(dimension)

    # vertex_labels = ["".join(str(digit) for digit in code) for code in vertex_codes]
    vertex_labels = [str(vid ^ subst) for vid in vertex_ids]

    fig = plt.figure(figsize=(12, 12))
    ax = fig.add_axes([0, 0, 1, 1])
    draw_graph(ax, edges, vertex_points, vertex_labels)
    fig.savefig("output.png")


def binary_code(numbers, width):
    """
    Encode numbers as binary arrays of given width.
    """
    codes = []
    for number in numbers:
        code = [(number >> i) & 1 for i in reversed(range(width))]
        codes.append(code)
    return np.array(codes)


def hypercube(dimension):
    """
    Generate hypercube edges. Returns a list of pairs of joined vertices. Each
    vertex is identified by its integral code.
    """
    edges = []
    for n in range(dimension):
        vertices = 2 ** n
        new_edges = edges.copy()
        new_edges += [(i + vertices, j + vertices) for i, j in edges]
        new_edges += [(i, i + vertices) for i in range(vertices)]
        edges = new_edges
    return edges


def draw_graph(ax, edges, points, labels):
    label_margin = 0.05
    centroid = points.mean(0)

    ax.scatter(
        points[:, 0],
        points[:, 1],
        lw=0.5,
        s=40,
        edgecolor="#222",
        facecolor="#f44",
        zorder=2,
    )

    for point, label in zip(points, labels):
        outer = point - centroid
        outer = outer / np.linalg.norm(outer)

        ax.text(
            point[0] + outer[0] * label_margin,
            point[1] + outer[1] * label_margin,
            label,
            ha="center",
            va="center",
            fontsize=10,
            color="#5a4",
            zorder=3,
        )

    for i, j in edges:
        ax.plot(
            [points[i, 0], points[j, 0]],
            [points[i, 1], points[j, 1]],
            lw=0.5,
            color="#aaa",
            zorder=1,
        )

    ax.set_aspect("equal")
    ax.set_xticks([])
    ax.set_yticks([])
    for key in ax.spines:
        ax.spines[key].set_visible(False)


main()
