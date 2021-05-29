import h5py
import matplotlib.pyplot as plt


with h5py.File("_output.h5", "r") as store:
    snapshots = store["snapshots"]

    spins_a = snapshots[0]
    spins_b = snapshots[len(snapshots) // 10]
    spins_c = snapshots[-1]

    fig, axes = plt.subplots(1, 3, figsize=(6, 2), dpi=150)

    ax = axes[0]
    ax.matshow(spins_a, vmin=-1, vmax=1, cmap="gray")
    ax.set_xticks([])
    ax.set_yticks([])

    ax = axes[1]
    ax.matshow(spins_b, vmin=-1, vmax=1, cmap="gray")
    ax.set_xticks([])
    ax.set_yticks([])

    ax = axes[2]
    ax.matshow(spins_c, vmin=-1, vmax=1, cmap="gray")
    ax.set_xticks([])
    ax.set_yticks([])

    fig.savefig("simulation.png", transparent=False)
