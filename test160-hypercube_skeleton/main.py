import matplotlib.pyplot as plt
import numpy as np


def main():
    # I don't understand the theory, but from my inspection the Petrie
    # projection of n-dimensional hypercube looks to be successively
    # constructed in the following way. Given w outer vertices of the
    # projection of Qn-1, use consecutive w/2 + 1 vertices as half the outer
    # vertices of the projection of Qn. The remaining half is filled in the
    # same way but from a copy of another projection of Qn-1 (note: Qn is
    # two copies of Qn-1 joined together). The untaken w/2 - 1 vertices goes
    # one layer inside.
    #
    # I guess the same procedure might work in the inner layers.
    #
    # So, the number of vertices in the outermost layer Vn(1) and second layer
    # Vn(2) should scale like this:
    #
    #   Vn(1) = (Vn-1(1) / 2 + 1) * 2
    #         = Vn-1(1) + 2 .
    #
    #   Vn(2) = (Vn-1(1) / 2 - 1) * 2 + (Vn-1(2) / 2 + 1) * 2
    #         = Vn-1(1) + Vn-1(2) .
    #
    #   n       1   2   3   4   5   ...
    #   Vn(1)   2   4   6   8   10? ...
    #   Vn(2)   0   0   2   8   16? ...
    #
    # Huh, if I take into account "non-carryable" cases, the algorithm produces
    # the following sequence instead of the above one:
    #
    #   n       1   2   3   4   5   ...
    #   Vn(1)   2   4   6   8   10  ...
    #   Vn(2)   0   0   2   6   10  ...
    #   Vn(3)   0   0   0   2   8   ...
    #   Vn(3)   0   0   0   0   4   ...
    #
    # It definitely does not follow the pattern of the Petrie projections, but
    # the total number of vertices is correct (2^n) and the embedding looks
    # kinda cool anyway.

    n = 5

    k = 1
    vertices = [2]

    print(vertices)

    while k < n:
        layer = vertices[0]

        # Halve vertices into next-outer and next-inner ones.
        taken = layer // 2 + 1
        carry = layer - taken

        if carry < 0:
            # If the number of vertices in the layer is so small that
            # "carrying" cannot happen, do not halve the vertices.
            carry = 0
            taken = layer

        # Join two copies.
        taken *= 2
        carry *= 2

        # Start from the outermost layer and go inside.
        next_vertices = [taken]

        for i in range(1, len(vertices)):
            layer = vertices[i] + carry // 2
            taken = layer // 2 + 1
            carry = layer - taken

            if carry < 0:
                taken = layer
                carry = 0

            taken *= 2
            carry *= 2

            next_vertices.append(taken)

        if carry > 0:
            next_vertices.append(carry)

        k += 1
        vertices = next_vertices

        print(vertices)

    # I doubt it's the correct Petrie projection, but at least it's a valid
    # projection because my algorithm produces the correct number of vertices.
    assert sum(vertices) == 2 ** n

    # Now we have the number of vertices in each layer. Let's layout them in
    # an arbitrarily symmetric way.
    fig, ax = plt.subplots(figsize=(5, 5))

    for i, layer in enumerate(vertices):
        radius = len(vertices) - i
        angles = np.linspace(0, 2 * np.pi, layer, endpoint=False)
        shift = np.pi / 2 * i
        xs = radius * np.cos(np.pi / 2 - angles - shift)
        ys = radius * np.sin(np.pi / 2 - angles - shift)
        ax.scatter(xs, ys, lw=0.5, s=40, edgecolor="black")
        shift += np.pi / 2

    ax.set_aspect("equal")
    ax.set_title(f"Unlabeled embedding of $ Q_{n} $")

    fig.savefig("output.png")



main()
