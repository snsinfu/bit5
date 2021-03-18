# Solve tridiagonal system of equations:
#
# [ D[0] U[0]                 ] [ X[0]   ]   [ B[0]   ]
# [ L[1] D[1] U[1]            ] [ X[1]   ]   [ B[1]   ]
# [      L[2] D[2] U[2]       ] [ X[2]   ] = [ B[2]   ]
# [             :    :        ] [   :    ]   [   :    ]
# [             L[n-1] D[n-1] ] [ X[n-1] ]   [ B[n-1] ]
#
# Here, we assume that D may contain zeros. So, the naive tridiagonal algorithm
# does not work.

import scipy.linalg  # For reference


def main():
    xxx = 0
    U = [0.1, 0.1, 0.1, 0.2, 0.2, xxx]
    D = [0.0, 0.2, 0.1, 0.2, 0.3, 0.4]
    L = [xxx, 0.1, 0.1, 0.1, 0.2, 0.2]
    B = [1.0, 1.1, 1.2, 1.3, 1.1, 1.0]

    # Using hand-crafted solver below.
    x_mine = solve(U, D, L, B)
    print("Mine:\t", format_vector(x_mine))

    # Using SciPy's reference solver.
    A = [[xxx] + U[:-1], D, L[1:] + [xxx]]
    x_scipy = scipy.linalg.solve_banded((1, 1), A, B)
    print("SciPy:\t", format_vector(x_scipy))


def solve(U, D, L, B):
    """
    Solves tridiagonal system of equations.

    The arguments must be of the same length (say n). U is the upper diagonal,
    D is the diagonal, L is the lower diagonal and B is the right-hand side.
    The i-th element of each vector specifies the i-th linear equation, i = 0
    to n-1 from the top to the bottom. Therefore, L and U must be padded by
    arbitrary value at L[0] and U[n-1].

    Returns the solution as a vector of length n.
    """
    n = len(D)
    assert n > 0
    assert len(U) == n
    assert len(L) == n
    assert len(B) == n

    # The algorithm modifies the coefficients in-place, so make copies.
    U = list(U)
    D = list(D)
    L = list(L)
    B = list(B)

    for i in range(n - 1):
        if abs(D[i]) >= abs(L[i + 1]):
            # Tridiagonal algorithm.
            # https://en.wikipedia.org/wiki/Tridiagonal_matrix_algorithm
            w = L[i + 1] / D[i]
            D[i + 1] -= w * U[i]
            B[i + 1] -= w * B[i]
            L[i + 1] = 0
        else:
            # Interchange the i-th row and the (i+1)-th row, then eliminate.
            # Unlike the other branch, the lower triangular element L[i+1] is
            # not eliminated. This affects the back substitution below.
            w = D[i] / L[i + 1]

            u = D[i + 1]
            D[i] = L[i + 1]
            D[i + 1] = U[i] - w * u
            L[i + 1] = U[i + 1]
            U[i + 1] *= -w
            U[i] = u

            b = B[i]
            B[i] = B[i + 1]
            B[i + 1] = b - w * B[i + 1]

    # Back substitution.
    B[n - 1] /= D[n - 1]
    for i in reversed(range(n - 1)):
        if i == n - 2:
            B[i] -= U[i] * B[i + 1]
        else:
            B[i] -= U[i] * B[i + 1] + L[i + 1] * B[i + 2]
        B[i] /= D[i]

    return B


def format_vector(vec):
    return "\t".join(f"{x:.1f}" for x in vec)


if __name__ == "__main__":
    main()
