# Solve ODE at t = tau > 0:
#
#   du/dt = a exp(-u/2) - b exp(u/2)
#
# where a, b > 0 and u(0) is given.

from math import sqrt, log, exp


def main():
    params_set = [
        {"a": 1, "b": 2, "u0": 0, "tau": 1},
        {"a": 1, "b": 2, "u0": -2, "tau": 1},
        {"a": 1, "b": 2, "u0": 2, "tau": 1},

        {"a": 2, "b": 1, "u0": 0, "tau": 1},
        {"a": 2, "b": 1, "u0": -2, "tau": 1},
        {"a": 2, "b": 1, "u0": 2, "tau": 1},

        {"a": 0, "b": 1, "u0": 0, "tau": 1},
        {"a": 0, "b": 1, "u0": -2, "tau": 1},
        {"a": 0, "b": 1, "u0": 2, "tau": 1},

        {"a": 1, "b": 0, "u0": 0, "tau": 1},
        {"a": 1, "b": 0, "u0": -2, "tau": 1},
        {"a": 1, "b": 0, "u0": 2, "tau": 1},

        {"a": 0, "b": 0, "u0": 0, "tau": 1},
        {"a": 0, "b": 0, "u0": -2, "tau": 1},
        {"a": 0, "b": 0, "u0": 2, "tau": 1},
    ]
    tolerance = 1e-3

    for params in params_set:
        u_n = solve_numerical(steps=10000, **params)
        u_s = solve_symbolical(**params)

        err = abs(u_s - u_n)
        if err < tolerance:
            condition = "ok"
        else:
            condition = "error"

        print("===", params, "=>", condition)
        print("numerical:", u_n)
        print("exact:    ", u_s)
        print("")


def solve_numerical(*, a, b, u0, tau, steps):
    dt = tau / steps
    u = u0
    for _ in range(steps):
        u += (a * exp(-u / 2) - b * exp(u / 2)) * dt
    return u


def solve_symbolical(*, a, b, u0, tau):
    if a == 0:
        return -2 * log(exp(-u0 / 2) + b * tau / 2)

    if b == 0:
        return 2 * log(exp(u0 / 2) + a * tau / 2)

    c = sqrt(a / b)
    z0 = exp(u0 / 2)
    k = (z0 + c) / (z0 - c) * exp(b * c * tau)
    z = c + 2 * c / (k - 1)
    u = 2 * log(z)
    return u


main()
