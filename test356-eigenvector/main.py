import numpy as np
import scipy.linalg


# Let J be an n-by-n matrix. Consider a sequence of n-vectors {s(t)} generated
# by the following rule:
#
#   s(t+1) = J s(t) .
#
# Diagonalizing J = PDP' allows one to easily predict t-th vector:
#
#   s(t) = P D^t P' s(0).
#
# Polar representation of eigenvalue λ = r e^(iα) tells the behavior of the
# corresponding mode. That is, the mode decays or grows depending on the
# magnitude r and cycles at the period 2π/α.

# Now, let's extend the rule to a continuous time regime. First, transform the
# discrete-time rule into a difference equation:
#
#   s(t+1) - s(t) = J s(t) - s(t) = -L s(t),   L = I - J.
#
# This suggests the following infinitesimal update rule:
#
#   s(t+dt) - s(t) = -L s(t) γ dt,
#
# so that
#
#   ds(t)
#   ----- = -γL s(t).
#     dt
#
# Diagonalization L = QEQ' gives
#
#   s(t) = Q exp(-γEt) Q' s(0) .
#
# Now, the behavior of a mode is governed by the real and the imaginary parts
# of the corresponding eigenvalue λ = η + iω. The sign of the real part η tells
# if the mode grows or decays. The imaginary part ω tells the frequency of an
# oscillating mode.


def main():
    J = np.array([
        [ 0,  0,  0,  0, 1,  0, 0, 0,  0,  0,  0, 0, 0],
        [-1,  0,  0,  0, 0,  0, 0, 0,  0,  0,  0, 0, 0],
        [ 0, -1,  0,  0, 0,  1, 0, 0,  0,  0,  0, 0, 0],
        [ 0,  0,  0,  0, 0,  0, 0, 0,  0,  0,  0, 0, 0],
        [ 0,  0,  0,  0, 0,  1, 0, 0,  0,  0,  0, 0, 0],
        [-1,  0,  0,  0, 0,  0, 0, 0,  0,  0,  0, 0, 0],
        [ 0,  0, -1,  0, 0, -1, 0, 0,  0,  0, -1, 0, 0],
        [ 0,  0,  0,  1, 0,  0, 0, 0,  1,  0,  0, 0, 0],
        [ 0,  0,  0, -1, 0,  0, 0, 0,  0,  0,  0, 0, 0],
        [ 0,  0,  0,  0, 0,  0, 0, 0,  0,  0,  0, 0, 0],
        [ 0,  0,  0,  0, 0,  1, 0, 0,  0, -1,  0, 0, 0],
        [ 0,  0,  0,  1, 0,  0, 0, 0,  0,  0, -1, 0, 1],
        [ 0,  0,  0,  0, 0,  0, 0, 0, -1,  0,  0, 0, 0],
    ])
    J = normalize_in(J)
    L = np.eye(len(J)) - J

    print("--- J ---")
    print_eigenmodes(J)

    print("")
    print("--- L ---")
    print_eigenmodes(L)

    print("")

    s = np.ones(len(J))
    print(format_spins(s))

    for _ in range(20):
        s = J @ s
        print(format_spins(s))


def normalize_in(J):
    """
    Normalize input weight of a signed adjacency matrix to one.
    """
    D = np.sum(np.abs(J), axis=1)
    D[D == 0] = 1
    J = J / D[:, None]
    return J


def print_eigenmodes(L):
    vals, vecs = np.linalg.eig(L)

    rows = []

    for i in np.arange(len(vals)):
        val = vals[i]
        vec = vecs[:, i]
        rows.append(format_complex(val) + " : " + format_vector(vec))

    print("\n".join(sorted(set(rows))))


def format_complex(c, epsilon=1e-6):
    if abs(c.real) < epsilon:
        s = "0"
    else:
        s = f"{c.real:.2g}"
    if abs(c.imag) >= epsilon:
        if s == "0":
            s = f"{c.imag:.2g}i"
        else:
            s += f"{c.imag:+.2g}i"
    return s


def format_vector(v):
    return "  ".join(format_complex(vi) for vi in v)


def format_spins(s):
    return "".join("-.+"[int(1 + np.sign(si))] for si in s)


main()
