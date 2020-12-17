import matplotlib.pyplot as plt
import numpy as np


step, time, pray, predator = np.loadtxt("_output.dat").T

plt.switch_backend("Agg")
fig, ax = plt.subplots(figsize=(4, 4))

ax.plot(pray, predator, lw=0.5, color="dimgray")
ax.scatter(pray, predator, c=time, lw=0, s=1, cmap="rainbow")

ax.set_xlabel("Prey")
ax.set_ylabel("Predator")

ax.spines["top"].set_visible(False)
ax.spines["right"].set_visible(False)

fig.savefig("_output.png", dpi=150)
