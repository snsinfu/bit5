import numpy as np
import sklearn.cluster


def main():
    adjacency = np.array([
        [0, 1, 1, 0, 0, 0, 0],
        [1, 0, 1, 0, 0, 0, 0],
        [1, 1, 0, 1, 0, 0, 0],
        [0, 0, 1, 0, 1, 1, 1],
        [0, 0, 0, 1, 0, 1, 1],
        [0, 0, 0, 1, 1, 0, 1],
        [0, 0, 0, 1, 1, 1, 0],
    ])

    use_sklearn(adjacency, 3)
    use_manual(adjacency, 3)


def use_sklearn(adjacency, n_clusters):
    labels = sklearn.cluster.spectral_clustering(
        adjacency, n_clusters=n_clusters, assign_labels="discretize",
    )
    print(labels)


def use_manual(adjacency, n_clusters):
    # Spectral embedding
    spectral_dim = n_clusters + 1
    laplacian = np.diag(adjacency.sum(1)) - adjacency
    eigvals, eigvecs = np.linalg.eigh(laplacian)
    embedding = eigvecs[:, :spectral_dim]
    # Then, K-means
    _, labels, _ = sklearn.cluster.k_means(embedding, n_clusters=n_clusters)
    print(labels)


main()
