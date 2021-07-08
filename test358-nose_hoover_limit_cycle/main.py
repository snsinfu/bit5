import numpy as np


def main():
    J = np.array([
        [0, 0, -1],
        [-1, 0, 0],
        [0, -1, 0],
    ])

    # J = np.array([
    #     [0, -1, 0, 0, 0],
    #     [0, 0, -1, 0, -1],
    #     [0, 0, 0, 1, 0],
    #     [0, 0, 0, 0, 1],
    #     [-1, 0, 0, -1, 0],
    # ])

    #J = np.array([
    #    [ 0,  0,  0,  0, 1,  0, 0, 0,  0,  0,  0, 0, 0],
    #    [-1,  0,  0,  0, 0,  0, 0, 0,  0,  0,  0, 0, 0],
    #    [ 0, -1,  0,  0, 0,  1, 0, 0,  0,  0,  0, 0, 0],
    #    [ 0,  0,  0,  0, 0,  0, 0, 0,  0,  0,  0, 0, 0],
    #    [ 0,  0,  0,  0, 0,  1, 0, 0,  0,  0,  0, 0, 0],
    #    [-1,  0,  0,  0, 0,  0, 0, 0,  0,  0,  0, 0, 0],
    #    [ 0,  0, -1,  0, 0, -1, 0, 0,  0,  0, -1, 0, 0],
    #    [ 0,  0,  0,  1, 0,  0, 0, 0,  1,  0,  0, 0, 0],
    #    [ 0,  0,  0, -1, 0,  0, 0, 0,  0,  0,  0, 0, 0],
    #    [ 0,  0,  0,  0, 0,  0, 0, 0,  0,  0,  0, 0, 0],
    #    [ 0,  0,  0,  0, 0,  1, 0, 0,  0, -1,  0, 0, 0],
    #    [ 0,  0,  0,  1, 0,  0, 0, 0,  0,  0, -1, 0, 1],
    #    [ 0,  0,  0,  0, 0,  0, 0, 0, -1,  0,  0, 0, 0],
    #])

    J = np.array([
        [ 0,  0,  0,  0, 1,  0, 0, 0,  0,  0,  0, 0, 0],
        [-1,  0,  0,  0, 0,  0, 0, 0,  0,  0,  0, 0, 0],
        [ 0, -1,  0,  0, 0,  1, 0, 0,  0,  0,  0, 0, 0],
        [ 0,  0,  0,  0, 0,  0, 0, 1,  0,  0,  0, 0, 0],
        [ 0,  0,  0,  0, 0,  1, 0, 0,  0,  0,  0, 0, 0],
        [-1,  0,  0,  0, 0,  0, 0, 0,  0,  0,  0, 0, 0],
        [ 0,  0, -1,  0, 0, -1, 0, 0,  0,  0, -1, 0, 0],
        [ 0,  0,  0,  0, 0,  0, 0, 0,  1,  0,  0, 0, 0],
        [ 0,  0,  0, -1, 0,  0, 0, 0,  0,  0,  0, 0, 0],
        [ 0,  0,  0,  0, 0,  0, 0, 0,  0,  0,  0, 0, 0],
        [ 0,  0,  0,  0, 0,  1, 0, 0,  0, -1,  0, 0, 0],
        [ 0,  0,  0,  1, 0,  0, 0, 0,  0,  0, -1, 0, 1],
        [ 0,  0,  0,  0, 0,  0, 0, 0, -1,  0,  0, 0, 0],
    ])

    J = normalize_in(J)

    params = {
        "omega": 5,
        "beta":  1,
        "dt":    0.01,
        "seed":  0,
        "steps": 10000,
    }

    for step, s, v in nose_hoover(J, **params):
        if step % 10 == 0:
            t = params["dt"] * step
            print_state(t, s, v)


def normalize_in(J):
    D = np.sum(np.abs(J), axis=1)
    D[D == 0] = 1
    J = J / D[:, None]
    return J


def nose_hoover(J, *, omega, beta, dt, seed, steps):
    random = np.random.Generator(np.random.PCG64(seed))
    n = len(J)
    L = J - np.eye(n)
    s = np.zeros(n)
    v = random.normal(size=n)
    gamma = 0

    yield 0, s, v

    for step in range(1, steps + 1):
        # Nose-Hoover thermostat
        gamma += omega**2 * (beta * np.sum(v ** 2) - 1) * dt
        v += (L @ s - gamma * v) * dt
        s += v * dt

        yield step, s, v


def print_state(t, s, v):
    row = f"{t:.3g}"
    row += "  " + format_state(s)
    row += "  " + format_state(v)
    print(row)


def format_state(s):
    return "  ".join(f"{si:.3g}" for si in s)


main()
