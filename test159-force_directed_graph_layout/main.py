import matplotlib.pyplot as plt
import numpy as np


def main():
    edges = [
        (0, 1), (1, 2), (2, 3), (3, 0),
        (4, 5), (5, 6), (6, 7), (7, 4),
        (0, 4), (1, 5), (2, 6), (3, 7),

        (8, 9), (9, 10), (10, 11), (11, 8),
        (12, 13), (13, 14), (14, 15), (15, 12),
        (8, 12), (9, 13), (10, 14), (11, 15),

        (0, 8), (1, 9), (2, 10), (3, 11),
        (4, 12), (5, 13), (6, 14), (7, 15),
    ]
    points = compute_layout(16, edges)

    fig, ax = plt.subplots(figsize=(5, 5))

    for i, j in edges:
        ax.plot(
            [points[i, 0], points[j, 0]],
            [points[i, 1], points[j, 1]],
            lw=1, color="black", zorder=1
        )
    ax.scatter(
        points[:, 0],
        points[:, 1],
        lw=1, s=50, edgecolor="black", facecolor="red", zorder=2
    )

    bound = 1.2 * np.max(np.abs(points))
    ax.set_xlim(-bound, bound)
    ax.set_ylim(-bound, bound)
    ax.set_aspect("equal")

    fig.savefig("output.png")



def compute_layout(vertices, edges):
    dimension = 2

    points = np.empty((vertices, dimension), dtype=np.float32)
    forces = np.empty((vertices, dimension), dtype=np.float32)

    angles = np.linspace(0, 2 * np.pi, num=vertices, endpoint=False)
    angles += np.random.normal(0, 0.1, size=angles.shape)
    points[:, 0] = np.cos(angles)
    points[:, 1] = np.sin(angles)

    repulsion = 1.0
    stepsize = 0.01
    steps = 2000

    for step in range(steps):
        forces[:, :] = 0
        loss = 0

        for i in range(vertices):
            for j in range(i):
                delta = points[i] - points[j]
                delta_sq = np.dot(delta, delta)
                force = repulsion / delta_sq * normalize(delta)
                forces[i] += force
                forces[j] -= force

        for i, j in edges:
            delta = points[i] - points[j]
            spring = 1 - np.linalg.norm(delta)
            force = spring * normalize(delta)
            loss += 0.5 * spring**2
            forces[i] += force
            forces[j] -= force

        points += stepsize * forces

    points -= points.mean(axis=0, keepdims=True)
    print(loss)

    return points


def normalize(vec):
    return 1 / np.linalg.norm(vec) * vec


main()
