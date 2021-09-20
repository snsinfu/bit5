import numpy as np


MODE_ITERATIVE = 0
MODE_ALGEBRAIC = 1


class Config:
    nodes = 10
    connectivity = 0.1
    max_steps = 100
    replicates = 100
    rho = 0.8
    random_seed = 0
    mode = MODE_ALGEBRAIC


def main(config):
    random = np.random.Generator(np.random.PCG64(config.random_seed))

    regulation_matrix = generate_random_network(
        config.nodes, config.connectivity, random
    )
    adjacency_matrix = np.abs(regulation_matrix)

    samples = np.concatenate(
        [
            simulate_network(regulation_matrix, config.max_steps, random)
            for _ in range(config.replicates)
        ]
    )

    total_correlation = np.corrcoef(samples.T)
    direct_correlation = estimate_direct_correlation(
        total_correlation, adjacency_matrix, config.rho, config.mode
    )

    style = {"fmt": "%g", "delimiter": "\t"}
    np.savetxt("_samples.dat", samples, **style)
    np.savetxt("_regulation_matrix.dat", regulation_matrix, **style)
    np.savetxt("_total_correlation.dat", total_correlation, **style)
    np.savetxt("_direct_correlation.dat", direct_correlation, **style)


def estimate_direct_correlation(total_correlation, adjacency_matrix, rho, mode):
    if mode == MODE_ITERATIVE:
        return iterative_inversion(total_correlation, adjacency_matrix, rho)
    if mode == MODE_ALGEBRAIC:
        return algebraic_inversion(total_correlation, rho)
    assert False


def iterative_inversion(total_correlation, adjacency_matrix, rho):
    direct_correlation = adjacency_matrix * total_correlation
    while True:
        prev_direct_correlation = direct_correlation
        direct_correlation = (
            total_correlation - rho * direct_correlation @ total_correlation
        )
        direct_correlation *= adjacency_matrix

        delta = np.abs(direct_correlation - prev_direct_correlation).max()
        if delta < 1e-6:
            break

    return direct_correlation


def algebraic_inversion(total_correlation, rho):
    H = total_correlation - np.diag(np.diagonal(total_correlation))
    I = np.eye(H.shape[0])
    C = H @ np.linalg.inv(I + rho * H)
    return C


def simulate_network(regulation_matrix, max_steps, random):
    n = len(regulation_matrix)
    state = generate_random_state(n, random)
    history = [state]
    for step in range(max_steps):
        state = np.sign(regulation_matrix @ state)
        if np.abs(state - history[-1]).max() == 0:
            break
        history.append(state)
    return np.array(history)


def generate_random_state(n, random):
    return -1 + 2 * random.binomial(1, 0.5, size=n)


def generate_random_network(n, p, random):
    while True:
        matrix = np.zeros((n, n), dtype=int)
        for i in range(n):
            rep_targets, act_targets = generate_random_targets(n, p, random)
            matrix[i, rep_targets] = -1
            matrix[i, act_targets] = +1

        matrix -= np.diag(np.diagonal(matrix))

        if np.all(np.any(matrix != 0, axis=1)):
            break
    return matrix


def generate_random_targets(n, p, random):
    n_reps = random.binomial(n, p)
    n_acts = random.binomial(n, p)
    targets = random.permutation(n)[: n_reps + n_acts]
    rep_targets = targets[:n_reps]
    act_targets = targets[n_reps:]
    return rep_targets, act_targets


main(Config())
