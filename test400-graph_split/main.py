import matplotlib.pyplot as plt
import numpy as np


def main():
    edges = np.array([
        (0, 1), (0, 5), (1, 2), (2, 6), (3, 7), (3, 8), (3, 11), (4, 0), (5, 2),
        (5, 4), (5, 6), (5, 10), (8, 7), (8, 12), (9, 10), (10, 11), (12, 11),
    ])

    recursive_split(edges)


def recursive_split(edges, level=0):
    nodes = np.array(list(set(np.ravel(edges))))
    print(". " * level + format_nodes(nodes))

    if len(edges) <= 3:
        return

    mapping_back = nodes
    mapping_forw = np.full(nodes.max() + 1, -1, dtype=int)
    mapping_forw[nodes] = np.arange(len(nodes))

    a, b = split(mapping_forw[edges])
    a = mapping_back[a]
    b = mapping_back[b]

    a_edges = np.array([(i, j) for i, j in edges if i in a and j in a])
    b_edges = np.array([(i, j) for i, j in edges if i in b and j in b])

    recursive_split(a_edges, level + 1)
    recursive_split(b_edges, level + 1)


def split(edges):
    """
    Split connected graph into two densely connected components.
    """
    laplacian = make_laplacian(make_adjacency_matrix(edges))
    eigvals, eigvecs = np.linalg.eigh(laplacian)
    signals = eigvecs[:, 1]
    indices = np.arange(len(signals))
    a = indices[signals > 0]
    b = indices[signals < 0]
    return (a, b) if a[0] < b[0] else (b, a)


def make_laplacian(adjacency_matrix):
    """
    Compute graph laplacian from adjacency matrix.
    """
    return np.diag(adjacency_matrix.sum(1)) - adjacency_matrix


def make_adjacency_matrix(edges):
    """
    Create adjacency matrix from edge list.
    """
    n_nodes = count_nodes(edges)
    adjacency_matrix = np.zeros((n_nodes, n_nodes))
    for i, j in edges:
        adjacency_matrix[i, j] = 1
        adjacency_matrix[j, i] = 1
    return adjacency_matrix


def count_nodes(edges):
    """
    Deduce the number of nodes from edge list.
    """
    return max(max(i, j) for i, j in edges) + 1


def format_nodes(nodes):
    """
    Format list of node indices as concise string.
    """
    return " ".join(str(i) for i in nodes)


main()
