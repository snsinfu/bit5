import numpy as np


# H(q, p) = 1/2 (p - Aq)^2 - pAq
# v =  ∂H/∂p = p
# f = -∂H/∂q = -A'Aq

# A = np.array([
#     [0, -1, 0],
#     [0, 0, -1],
#     [-1, 0, 0],
# ])

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

vals, P = np.linalg.eigh(J.T @ J)
print(vals)
print(P)
exit()


# Direct or Laplacian?
A = J
# A = J - np.diag(np.sum(np.abs(J), axis=1))

q = np.zeros(len(A))
p = np.zeros(len(A))

# q[3] = 1
# q[9] = 1
# p = np.random.normal(0, 1, size=q.shape)

kT = 1.0
dt = 0.01
steps = 20000
seed = 1

random = np.random.Generator(np.random.PCG64(seed))


for step in range(steps):
    t = dt * step
    f = -A.T @ A @ q

    gamma = 1 / (1 + t)
    f += -gamma * p
    f += np.sqrt(2 * kT * gamma * dt) * random.normal(0, 1, size=p.shape)

    p += f * dt
    q += p * dt

    if steps % 10 == 0:
        row = [f"{t:g}"]
        row += [f"{qi:.3g}" for qi in P.T@q]
        row += [f"{pi:.3g}" for pi in p]
        print("\t".join(row))
