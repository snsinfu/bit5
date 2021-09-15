import numba
import numpy as np

r_starts, r_ends, g = np.loadtxt("_rdf.dat").T

n_bins = len(g)
r_centers = (r_starts + r_ends) / 2
r_step = r_ends[0] - r_starts[0]
r_max = r_ends[-1]

integral_divisions = 100
max_iterations = 50
n_particles = 1000
box_size = 1.0
density = n_particles / box_size**3

# Behrens and Grier (2001)
# doi:10.1103/PhysRevE.64.050401

I = np.zeros(n_bins)

theta_mesh = np.linspace(0, np.pi, integral_divisions, endpoint=False)
phi_mesh = np.linspace(0, 2 * np.pi, integral_divisions, endpoint=False)
s_mesh = np.linspace(0, r_max, integral_divisions, endpoint=False)

d_theta = theta_mesh[1] - theta_mesh[0]
d_phi = phi_mesh[1] - phi_mesh[0]
d_s = s_mesh[1] - s_mesh[0]


@numba.njit
def evaluate_integral(r, prev_Ir):
    Ir = 0

    for s in s_mesh:
        distance_bin = int(s / r_step)
        radial_factor = g[distance_bin] - 1 - density * prev_Ir
        kernel_factor = 0

        for theta in theta_mesh:
            area_element = s**2 * np.sin(theta) * d_theta * d_phi
            for phi in phi_mesh:
                sx = s * np.sin(theta) * np.cos(phi)
                sy = s * np.sin(theta) * np.sin(phi)
                sz = s * np.cos(theta)
                distance = np.sqrt(sx**2 + sy**2 + (sz - r)**2)
                distance_bin = int(distance / r_step)
                if distance_bin < n_bins:
                    kernel_factor += (g[distance_bin] - 1) * area_element

        Ir += radial_factor * kernel_factor * d_s

    return Ir



for iteration in range(max_iterations):
    I_prev = I.copy()

    for i in range(n_bins):
        r = r_centers[i]
        I[i] = evaluate_integral(r, I_prev[i])

    max_delta = np.abs(I - I_prev).max()
    print(f"{iteration}: delta = {max_delta:g}")


nI = density * I
w = -np.log(g)
u = w + nI

import matplotlib.pyplot as plt

plt.plot(r_centers, w)
plt.plot(r_centers, u)
plt.show()

