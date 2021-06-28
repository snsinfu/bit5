# Microcanonical Ising model simulation. This uses the Deutsch 2008 scheme
# where each spin has an auxiliary energy variable (kinetic energy).

import numpy as np


random_seed = 0
n_spins = 100
steps = 1000000
output_interval = 100
interaction = 1
init_kT = 0.5


random = np.random.Generator(np.random.PCG64(random_seed))
spins = np.full(n_spins, -1)
demons = np.zeros(n_spins)

# Hot spin
demons[n_spins // 2] = init_kT * n_spins


def output():
    spin_text = " ".join(f"{int(s > 0)}" for s in spins)
    demon_text = " ".join(f"{e:.3g}" for e in demons)
    print(spin_text, demon_text)


output()


for step in range(steps):
    i = random.integers(n_spins)
    left = (i + n_spins - 1) % n_spins
    right = (i + 1) % n_spins

    move = random.integers(2)

    # Flip a spin.
    if move == 0:
        delta_e = -2 * interaction * spins[i] * (spins[left] + spins[right])
        if demons[i] + delta_e >= 0:
            spins[i] = -spins[i]
            demons[i] += delta_e

    # Exchange kinetic energy with a neighbor.
    if move == 1:
        j = random.choice([left, right])
        e = demons[i] + demons[j]
        r = random.uniform(0, 1)
        demons[i] = r * e
        demons[j] = (1 - r) * e

    # Output.
    if (step + 1) % output_interval == 0:
        output()
