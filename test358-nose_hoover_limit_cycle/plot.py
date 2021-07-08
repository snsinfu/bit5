import matplotlib.colors
import matplotlib.pyplot as plt
import numpy as np


c_red = "#ee5555"
c_green = "#55cc33"
c_blue = "#4466dd"
c_bky = ["#6666ff", "#000000", "#ffcc00"]
cmap_sign = matplotlib.colors.LinearSegmentedColormap.from_list("", c_bky)


trajectory = np.loadtxt("_output.dat")
n = (trajectory.shape[1] - 1) // 2
t = trajectory[:, 0]
s = trajectory[:, 1:(n + 1)]
v = trajectory[:, (n + 1):]


fig, ax = plt.subplots(figsize=(5.5, 1.5), dpi=120)
ax.matshow(s.T, vmin=-1, vmax=1, cmap=cmap_sign, aspect="auto")

# fig, ax = plt.subplots(figsize=(5.5, 1), dpi=120)
# ax.plot(s[:, 0], lw=0.8, color=c_red)
# ax.plot(s[:, 1], lw=0.8, color=c_green)
# ax.plot(s[:, 2], lw=0.8, color=c_blue)

# fig, ax = plt.subplots(figsize=(3, 3), dpi=120)
# ax.plot(s[:, 0], s[:, 1], lw=0.8, color=c_red)
# ax.plot(s[:, 1], s[:, 2], lw=0.8, color=c_green)
# ax.plot(s[:, 2], s[:, 0], lw=0.8, color=c_blue)
# ax.set_aspect("equal")

plt.show()
