import h5py
import numba
import numpy as np


start_frame = 100
box_size = 1.0
bin_width = 0.005


with h5py.File("simulation/_output.h5", "r") as store:
    points_samples = store["positions_history"][start_frame:]

cutoff_distance = box_size / 2
n_samples, n_points, _ = points_samples.shape
n_bins = int(cutoff_distance / bin_width)

reference_density = (n_points - 1) / box_size**3


@numba.njit
def collect_distances(points, bin_freqs, box_size):
    n = len(points)
    for i in range(n):
        for j in range(i + 1, n):
            dx, dy, dz = points[i] - points[j]
            dx = dx - round(dx / box_size) * box_size
            dy = dy - round(dy / box_size) * box_size
            dz = dz - round(dz / box_size) * box_size
            distance = np.sqrt(dx**2 + dy**2 + dz**2)

            loc = int(distance / bin_width)
            if loc < len(bin_freqs):
                bin_freqs[loc] += 1


bin_freqs = np.zeros(n_bins)
bin_edges = np.arange(n_bins + 1) * bin_width
bin_volumes = (bin_edges[1:]**3 - bin_edges[:-1]**3) * np.pi * 4 / 3

for points in points_samples:
    collect_distances(points, bin_freqs, box_size)

bin_densities = bin_freqs / bin_volumes * 2 / n_points / n_samples
bin_rdf = bin_densities / reference_density

data = np.transpose([bin_edges[:-1], bin_edges[1:], bin_rdf])
np.savetxt("_rdf.dat", data, fmt="%g", delimiter="\t")
