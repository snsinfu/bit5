import matplotlib.pyplot as plt
import numpy as np


t, H, x, y, qx, qy, px, py = np.loadtxt("_output.dat").T

fig, ax = plt.subplots(figsize=(2.5, 2.5), dpi=150)

ax.plot(x, y, lw=0.4)
ax.set_xticks([0, 1, 2])
ax.set_yticks([0, 1, 2])

plt.show()
