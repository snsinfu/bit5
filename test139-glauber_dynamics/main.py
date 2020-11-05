# https://sarcasticresonance.wordpress.com/2017/07/14/random-graphs-the-exponential-random-graph-model/

import numpy as np


num_vertices = 30
glauber_steps = 10000
connection_probability = 0.2
random = np.random.RandomState(0)

# Adding/removing an edge increases/decreases hamiltonian by this amount.
edge_hamiltonian = np.log(connection_probability / (1 - connection_probability))

adjacency_matrix = np.zeros((num_vertices, num_vertices), dtype=int)

for _ in range(glauber_steps):
    while True:
        i, j = random.randint(0, num_vertices, size=2)
        if i < j:
            break

    deletion_probability = 1 / (1 + np.exp(edge_hamiltonian))

    if random.uniform(0, 1) < deletion_probability:
        edge_state = 0
    else:
        edge_state = 1

    adjacency_matrix[i, j] = edge_state
    adjacency_matrix[j, i] = edge_state


degrees = adjacency_matrix.sum(1)
mean_degree = degrees.mean()

print(f"Mean degree: {mean_degree:g}")
