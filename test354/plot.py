import matplotlib.pyplot as plt
import numpy as np


plt.cm.register_cmap(
    cmap=plt.cm.colors.LinearSegmentedColormap.from_list(
        "p:sign", [(0.1, 0, 0.7), (0, 0, 0), (0.8, 0, 0.1)],
    )
)


trajectory = np.loadtxt("_output.dat")
n = (trajectory.shape[1] - 1) // 2

t = trajectory[:, 0]
q = trajectory[:, 1:(n + 1)]
p = trajectory[:, (n + 1):]

# fig, ax = plt.subplots(figsize=(2.5, 2.5), dpi=150)
# ax.plot(q[:, 0], p[:, 0], lw=0.4)
# ax.plot(q[:, 0], q[:, 1], lw=0.4)
# ax.plot(q[:, 0], q[:, 2], lw=0.4)
# ax.set_aspect("equal")


def signed_log(x):
    return (np.sign(q) * np.log(1 + np.abs(q)))


fig, ax = plt.subplots(figsize=(5.5, 1.0), dpi=150)

X = signed_log(q)

ax.matshow(
    X.T,
    extent=[0, X.shape[0], n + 0.5, 0.5],
    vmin=-0.02, vmax=0.02, cmap="p:sign", aspect="auto",
    zorder=1,
)

for i in range(1, n):
    ax.axhline(0.5 + i, lw=0.4, color="#000000", zorder=2)

ax.set_yticks(np.arange(1, n + 1))

plt.show()
