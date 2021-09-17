import math
import numba
import numpy as np


def main():
    starts, ends, rdf = np.loadtxt("rdf.dat").T
    density = 1200 / 1.0**3

    n_bins = len(rdf)
    bin_width = ends[0] - starts[0]

    corrector = np.zeros(n_bins)
    kernel = compute_kernel(rdf, bin_width)

    for step in range(100):
        corrector = bin_width * kernel @ (rdf - 1 - density * corrector)

    direct_rdf = np.exp(np.log(rdf) - density * corrector)

    for r, raw_gr, direct_gr in zip(starts, rdf, direct_rdf):
        print(f"{r:.3f}\t{raw_gr:.3f}\t{direct_gr:.3f}")


def compute_kernel(rdf, bin_width):
    n_bins = len(rdf)
    kernel = np.zeros((n_bins, n_bins))

    @numba.njit
    def integrate(r, s, div=1000):
        theta_step = math.pi / div
        integral = 0

        for theta_bin in range(div):
            theta = theta_bin * theta_step
            distance = math.hypot(s * math.sin(theta), r - s * math.cos(theta))
            distance_bin = int(distance / bin_width)
            if distance_bin < n_bins:
                integral += (rdf[distance_bin] - 1) * math.sin(theta)

        integral *= 2 * math.pi * s**2 * theta_step

        return integral

    for r_bin in range(n_bins):
        for s_bin in range(n_bins):
            r = bin_width * r_bin
            s = bin_width * s_bin
            kernel[r_bin, s_bin] = integrate(r, s)

    return kernel


main()
