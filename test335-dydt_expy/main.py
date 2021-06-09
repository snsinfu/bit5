import math

k = 0.5
r = 0.3
beta = 0.1
u = 0.2
dt = 0.0001
steps = 1000000

for i in range(steps):
    v = k * math.exp(-beta * u) - r * math.exp(beta * u)
    u += v * dt

    if i % 1000 == 0:
        print(u)
