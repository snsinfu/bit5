# Obtain smallest n eigenvalues

# Power iteration
# https://en.wikipedia.org/wiki/Power_iteration
#
# Spectral shift
# https://math.stackexchange.com/a/271876
#
# Wielandt deflation
# https://math.stackexchange.com/a/1114855

import numpy as np


size = 1000
rate = 4 / size


def main():
    random = np.random.RandomState(0)

    matrix = random.binomial(1, rate, size=(size, size)).astype(np.float32)
    matrix = np.maximum(matrix, matrix.T)
    np.fill_diagonal(matrix, 0)

    sq_matrix = matrix @ matrix

    val, vec = power_iteration(
        sq_matrix,
        random.uniform(-1, 1, size=size),
        steps=100,
    )
    val_1 = val
    vec_1 = vec

    sq_matrix -= val_1 * np.eye(size, dtype=np.float32)

    val, vec = power_iteration(
        sq_matrix,
        random.uniform(-1, 1, size=size),
        steps=100,
    )
    val_2 = val + val_1
    vec_2 = vec

    print(vec_1 @ matrix @ vec_1)
    print(vec_2 @ matrix @ vec_2)  # Hmm...



def power_iteration(matrix, init, steps):
    vec = init / np.linalg.norm(init)
    for _ in range(steps):
        vec = matrix @ vec
        vec = vec / np.linalg.norm(vec)
    val = vec @ matrix @ vec
    return val, vec


main()
