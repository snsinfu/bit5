"""
Correlation-based weak adjacency of genes.
"""

import argparse
import os
import signal

import h5py
import numpy as np

from sklearn.metrics import pairwise_distances_chunked


CACHE_SIZE = 256 * 1024 * 1024
CHUNK_SIZE = 500
R2_THRESHOLD = 0.1
MAX_NEIGHBORS = 100


def main(*, gtex):
    store = h5py.File(gtex, "r", rdcc_nbytes=CACHE_SIZE)
    genes = list(store["genes"][:])
    expressions = store["expressions"]
    n_genes, n_cells = expressions.shape

    Z = transform_rows(expressions, lambda x: np.log10(1 + x), CHUNK_SIZE)
    Z = Z - np.mean(Z, axis=1)[:, None]

    gene_indices = np.arange(n_genes, dtype=np.int32)
    threshold = R2_THRESHOLD
    top_k = MAX_NEIGHBORS
    i = 0

    for chunk in pairwise_distances_chunked(Z, metric="cosine", working_memory=0):
        # Transform cosine distance to cosine correlation.
        score = 1 - chunk[0]

        # Adjust correlation against bias.
        score = score - score.mean()

        # Then, obtain a score similar to coefficient of determination.
        score = score**2

        # Do not count for self correlation.
        score[i] = 0

        tops = np.argsort(score)[::-1][:top_k]
        tops = tops[score[tops] > threshold]

        tops_text = " ".join(f"{j}" for j in tops)
        print(f"{i}: {tops_text}")

        i += 1


def transform_rows(table, func, chunk_size=1):
    data = []
    for beg in range(0, table.shape[0], chunk_size):
        end = min(beg + chunk_size, table.shape[0])
        data.extend(func(table[beg:end]))
    return np.array(data)


def parse_args():
    parser = argparse.ArgumentParser()
    parser.add_argument("gtex", type=str)
    return vars(parser.parse_args())


try:
    main(**parse_args())
except KeyboardInterrupt:
    signal.signal(signal.SIGINT, signal.SIG_DFL)
    os.kill(os.getpid(), signal.SIGINT)
