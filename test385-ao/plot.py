import h5py
import matplotlib.pyplot as plt
import numpy as np


with h5py.File("_output.h5", "r") as store:
    positions_history = store["positions_history"][:]

box_size = 1
contact_distance = 0.2
n_frames, n_points, _ = positions_history.shape

dists = positions_history[:, :, None, :] - positions_history[:, None, :, :]
dists = dists - np.round(dists / box_size) * box_size
dists = np.linalg.norm(dists, axis=3)

dists = dists[n_frames // 2:]
contact_matrix = (dists < contact_distance).mean(axis=0)

fig, ax = plt.subplots()
ax.matshow(contact_matrix, vmin=0, vmax=0.1, cmap="turbo")
plt.show()
