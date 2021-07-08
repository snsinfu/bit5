import matplotlib.pyplot as plt
import numpy as np


x, vx = 0.1, 0.2
y, vy = 0.1, 0.1
dt = 0.001
v0 = 0.4
gamma = 1
K = 2.0
damping_mode = 5

T = (vx**2 + vy**2) / 2
V = (x**2 + y**2) * K / 2

history = [[x, y, vx, vy, T, V]]


for step in range(1, 1 + 50000):
    fx = -K * x
    fy = -K * y

    if damping_mode == 1:
        fx -= gamma * np.sign(vx) * (abs(vx) - v0)
        fy -= gamma * np.sign(vy) * (abs(vy) - v0)

    if damping_mode == 2:
        fx -= gamma * vx * (1 - v0 / abs(vx))
        fy -= gamma * vy * (1 - v0 / abs(vy))

    if damping_mode == 3:
        fx -= gamma * vx * (abs(vx) / v0 - 1)
        fy -= gamma * vy * (abs(vy) / v0 - 1)

    if damping_mode == 4:
        fx -= gamma * vx * np.log(abs(vx) / v0)
        fy -= gamma * vy * np.log(abs(vy) / v0)

    if damping_mode == 5:
        vv = np.hypot(vx, vy)
        fx -= gamma * vx * np.log(vv / v0)
        fy -= gamma * vy * np.log(vv / v0)

    vx = vx + fx * dt
    vy = vy + fy * dt
    x = x + vx * dt
    y = y + vy * dt

    T = 1 / 2 * (vx**2 + vy**2)
    V = K / 2 * (x**2 + y**2)

    if step % 100 == 0:
        history.append([x, y, vx, vy, T, V])

xs, ys, vxs, vys, Ts, Vs = np.transpose(history)


fig, ax = plt.subplots(figsize=(3, 3), dpi=150)
ax.plot(xs, ys, lw=0.5, color="#666666", zorder=1)
ax.scatter(xs[-1:], ys[-1:], s=10, ec="none", fc="#ff0066", zorder=2)
ax.set_aspect("equal")

plt.show()
