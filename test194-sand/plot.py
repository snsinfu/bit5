import h5py
import matplotlib.pyplot as plt
import numpy as np


plt.cm.register_cmap(
    cmap=plt.cm.colors.LinearSegmentedColormap.from_list(
        "s:particle_type", ["black", "red"]
    )
)


with h5py.File("_output.h5", "r") as store:
    particle_types = store["particle_types"][:]
    positions_history = store["positions_history"][:]

export_history = (positions_history[:, :, 0] < 0).sum(axis=1)


fig, ax = plt.subplots(figsize=(5, 3))
ax.plot(export_history)
ax.set_xlabel("Time")
ax.set_ylabel("Export count")
ax.set_title("Export kinetics")

fig, ax = plt.subplots(figsize=(5, 5))
x = positions_history[1200, :, 0]
y = positions_history[1200, :, 1]
c = particle_types
ax.scatter(x, y, c=c, lw=0, s=3, cmap="s:particle_type")
ax.set_xlim(-3, 3)
ax.set_ylim(-2, 2)
ax.set_aspect("equal")

plt.show()
