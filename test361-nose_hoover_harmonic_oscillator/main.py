import math


def main():
    k = 2
    omega = 0.2
    beta = 1
    steps = 100000
    dt = 0.001

    x, vx = 1, 0.8
    y, vy = 1, 0.2
    gamma = 0

    for step in range(steps):
        t = step * dt
        kin = vx**2 + vy**2
        gamma += omega**2 * (beta * kin - 1)
        vx += (-k * x - gamma * vx) * dt
        vy += (-k * y - gamma * vy) * dt
        x += vx * dt
        y += vy * dt

        if step % 100 == 0:
            print(f"{t:g}\t{x:g}\t{y:g}")


main()
