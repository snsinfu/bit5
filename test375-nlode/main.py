import math

#
# du/dt = A φ(u) - B (1 - φ(u)) - ku
#

def main():
    u = -0.1
    dt = 0.01

    A = 0.3
    B = None
    k = 0.05
    beta = 5
    n_steps = 10_000

    print_state(0, u)

    for step in range(1, n_steps + 1):

        # Parameter schedule
        if step < 5000:
            B = 0.3
        else:
            B = -0.1

        phi = sigmoid(u, beta)
        rate = A * phi - B * (1 - phi) - k * u
        u += rate * dt

        if step % 10 == 0:
            print_state(step, u)


def print_state(step, u):
    print(f"{step}\t{u:g}")


def sigmoid(u, beta, epsilon=0.01):
    y = 1 / (1 + math.exp(-beta * u))
    y = (y - epsilon) / (1 - 2 * epsilon)
    return clip(y, 0, 1)


def clip(x, a, b):
    return min(max(x, a), b)


main()
