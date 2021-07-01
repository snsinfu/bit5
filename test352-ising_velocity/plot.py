import matplotlib.pyplot as plt
import numpy as np


t, a, b, u, v = np.loadtxt("_output.dat").T

fig, ax = plt.subplots(figsize=(5, 1.3))
ax.plot(t, a)
ax.plot(t, b)
plt.show()
