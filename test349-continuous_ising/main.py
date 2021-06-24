# Ising model energy:
#
#   U = -sum[i,j] Jij si sj .
#
# Let us use a continuous approximation of the spin states:
#
#   si = cos(xi)
#
# where xi is the continuous "coordinate" of the i-th spin.

import math
import random


n = 10
energy = -1
temperature = 0.01
mobility = 0.01
steps = 100
seed = 1


# Initialization

random.seed(seed)

interactions = [None] * n
states = [None] * n

for i in range(n):
    left = (i + n - 1) % n
    right = (i + 1) % n
    interactions[i] = [(left, energy), (right, energy)]
    states[i] = random.uniform(-math.pi, math.pi)


# Simulation

forces = [None] * n

for step in range(steps):
    # Update force
    for i in range(n):
        forces[i] = 0
        for j, e in interactions[i]:
            forces[i] += e * math.sin(states[i]) * math.cos(states[j])

    # Timestep
    for i in range(n):
        states[i] += mobility * forces[i]
        states[i] += math.sqrt(2 * temperature * mobility) * random.normalvariate(0, 1)

    # Output
    row = []
    for i in range(n):
        s = math.cos(states[i])
        row.append(f"{s:.1f}")
    print("\t".join(row))
