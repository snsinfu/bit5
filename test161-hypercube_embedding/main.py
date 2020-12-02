import matplotlib.pyplot as plt
import numpy as np


def main():
    # Actual embedding of the n-dimensional hypercube using the idea I used
    # in test160.
    n = 3

    k = 1
    layers = [[0, 1]]
    edges = [(0, 1)]

    def halve(vertices):
        m = min(len(vertices), len(vertices) // 2 + 1)
        return vertices[:m], vertices[m:]

    while k < n:
        # We join two copies of Qk together. This number gives the offset
        # of the indices of the vertices from the second copy.
        offset = 2 ** k

        # Start from the outermost layer and go inside.
        carry = []
        layers_next = []

        for i, layer in enumerate(layers):
            # Halve vertices into next-outer and next-inner ones.
            carry = [v for v in carry if v < offset]  # why?
            taken, carry = halve(layer + carry)

            # Join two copies.
            taken += [v + offset for v in reversed(taken)]
            carry += [v + offset for v in reversed(carry)]

            print("-->", taken, carry)

            layers_next.append(taken)

        if carry:
            layers_next.append(carry)

        edges_next = edges.copy()
        edges_next += [(i + offset, j + offset) for i, j in edges]
        edges_next += [(i, i + offset) for i in range(offset)]

        k += 1
        layers = layers_next
        edges = edges_next

    assert len(sum(layers, [])) == 2 ** n

    # Assign coordinates to the vertices.
    vertices = np.empty((2 ** n, 2))

    for i, layer in enumerate(layers):
        radius = (len(layers) - i)**2
        angles = np.linspace(0, 2 * np.pi, len(layer), endpoint=False)
        shift = np.pi / 2 * i
        xs = radius * np.cos(np.pi / 2 - angles - shift)
        ys = radius * np.sin(np.pi / 2 - angles - shift)
        for v, x, y in zip(layer, xs, ys):
            vertices[v, :] = (x, y)

    fig, ax = plt.subplots(figsize=(5, 5))

    for i, j in edges:
        ax.plot(
            [vertices[i, 0], vertices[j, 0]],
            [vertices[i, 1], vertices[j, 1]],
            lw=1, color="black", zorder=1,
        )

    ax.scatter(
        vertices[:, 0], vertices[:, 1],
        lw=0.5, s=40, marker="o", edgecolor="black", facecolor="red", zorder=2,
    )

    for v, (x, y) in enumerate(vertices):
        ax.text(
            x + 0.2, y - 0.2, f"{v}",
            ha="left", va="top", fontsize=14, fontweight="bold", color="red",
        )


    fig.savefig("output.png")


main()
