import matplotlib.pyplot as plt
import numpy as np


trajectory = np.loadtxt("_output.dat")
n = (trajectory.shape[1] - 1) // 2

t = trajectory[:, 0]
q = trajectory[:, 1:(n + 1)]
p = trajectory[:, (n + 1):]


# fig, ax = plt.subplots(figsize=(5.5, 1.0), dpi=150)
# ax.plot(t, q[:, 0], lw=0.5, color="#ff0066")
# ax.plot(t, q[:, 1], lw=0.5, color="#6600ff")

fig, ax = plt.subplots(figsize=(2.5, 2.5), dpi=150)
ax.plot(q[:, 0], q[:, 1], lw=0.5, color="#ff0066")
ax.plot(q[:, 1], q[:, 2], lw=0.5, color="#444444")
ax.plot(q[:, 2], q[:, 0], lw=0.5, color="#6600ff")

plt.show()
