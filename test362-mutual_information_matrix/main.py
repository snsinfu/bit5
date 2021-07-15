import numpy as np
import sklearn.metrics


# Compute pairwise mutual information of the features. The input is a data
# matrix of shape (observation, feature).

data = np.array([
    [-1, 1, 0, 1, -1],
    [1, 0, -1, 1, 1],
    [-1, 1, -1, 0, 1],
    [-1, 1, 0, 0, -1],
    [-1, 1, -1, -1, 1],
    [1, -1, -1, 1, 1],
])

chunks = sklearn.metrics.pairwise_distances_chunked(
    data.T,
    metric=sklearn.metrics.adjusted_mutual_info_score,
    working_memory=0,
)

for chunk in chunks:
    print("\t".join(f"{mi:.2g}" for mi in chunk[0]))
