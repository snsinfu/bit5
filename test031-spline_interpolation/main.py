def main():
    x = [0, 1, 2, 3, 4, 5]
    y = [1, 2, 0, 1, 2, 0]

    spline(x, y)


def spline(x, y):
    # http://fourier.eng.hmc.edu/e176/lectures/ch7/node6.html

    n = len(y)
    h = [None] + [x[i] - x[i - 1] for i in range(1, n)]

    def mu(i):
        return h[i] / (h[i + 1] + h[i])

    def lam(i):
        return 1 - mu(i)

    def double_diff(i):
        d1 = (y[i + 1] - y[i]) / h[i + 1]
        d2 = (y[i] - y[i - 1]) / h[i]
        return (d1 - d2) / (x[i + 1] - x[i - 1])

    A = [None] + [mu(i) for i in range(1, n - 1)] + [0]
    B = [2] * n
    C = [1] + [lam(i) for i in range(1, n - 1)] + [None]
    D = [0] + [6 * double_diff(i) for i in range(1, n - 1)] + [0]

    # https://en.wikipedia.org/wiki/Tridiagonal_matrix_algorithm

    M = [None] * n

    for i in range(1, n):
        w = A[i] / B[i - 1]
        B[i] -= w * C[i - 1]
        D[i] -= w * D[i - 1]

    M[n - 1] = D[n - 1] / B[n - 1]

    for i in reversed(range(n - 1)):
        M[i] = (D[i] - C[i] * M[i + 1]) / B[i]

    # curve(i, x) = ... + c x + d

    c = [None] * n
    d = [None] * n

    for i in range(1, n):
        c[i] = (y[i] - y[i - 1]) / h[i] - h[i] / 6 * (M[i] - M[i - 1])
        d[i] = (
            (x[i] * y[i - 1] - x[i - 1] * y[i]) / h[i]
            - h[i] / 6 * (x[i] * M[i - 1] - x[i - 1] * M[i])
        )

    def curve(i, xv):
        x3 = (x[i] - xv)**3 / (6 * h[i]) * M[i - 1]
        x2 = (xv - x[i - 1])**3 / (6 * h[i]) * M[i]
        x1 = c[i] * xv
        x0 = d[i]
        return x3 + x2 + x1 + x0

    def output(xv, yv):
        print(f"{xv:g}\t{yv:g}")

    for i in range(1, n):
        for j in range(10):
            u = j / 10
            xv = (1 - u) * x[i - 1] + u * x[i]
            output(xv, curve(i, xv))
    output(x[-1], curve(n - 1, x[-1]))

    # Check reproducibility

    tol = 1e-6
    for i in range(1, n):
        assert abs(curve(i, x[i - 1]) - y[i - 1]) < 1e-6
        assert abs(curve(i, x[i]) - y[i]) < 1e-6


if __name__ == "__main__":
    main()
