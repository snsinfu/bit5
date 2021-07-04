# Skew-symmetric Lotka-Volterra:
#
#   dx/dt = ax - kxy
#   dy/dt = by + kxy
#
# This can be translated to Hamiltonian formulation with the following
# generalized coordinates (qx, qy) and momenta (px, py):
#
#   qx = ∫ x(t) dt,  px = log x + k/2 qy ,
#   qy = ∫ y(t) dt,  py = log y - k/2 qx .
#
# And, the hamiltonian is
#
#   H = -a qx - b qy + x + y ,
#   x = exp(px - k/2 qy) ,
#   y = exp(py + k/2 qx) .
#
# The Hamiltonian equations of motion:
#
#   dqx/dt = exp(px - k/2 qy) ,
#   dqy/dt = exp(py + k/2 qx) ,
#   dpx/dt = a - k/2 exp(py + k/2 qx) ,
#   dpy/dt = b + k/2 exp(px - k/2 qy) .
#

from math import exp


a = 0.1
b = -0.1
k = 0.5

qx, px = 0, 0.5
qy, py = 0, -0.5

steps = 100000
dt = 0.01

for step in range(steps):
    vx = exp(px - k / 2 * qy)
    vy = exp(py + k / 2 * qx)
    Fx = a - k / 2 * vy
    Fy = b + k / 2 * vx

    qx += vx * dt
    qy += vy * dt
    px += Fx * dt
    py += Fy * dt

    x = exp(px - k / 2 * qy)
    y = exp(py + k / 2 * qx)
    H = x + y - a * qx - b * qy

    row = [
        f"{step:d}",
        f"{H:.3g}",
        f"{x:.3g}",
        f"{y:.3g}",
        f"{qx:.3g}",
        f"{qy:.3g}",
        f"{Fx:.3g}",
        f"{Fy:.3g}",
    ]
    print("\t".join(row))
