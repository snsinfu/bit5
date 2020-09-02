import scipy.sparse
import scipy.stats
import numpy as np


size = 1000
density = 5 / size

random = np.random.RandomState(0)

matrix = scipy.sparse.random(
    size,
    size,
    density,
    dtype=np.float32,
    random_state=random,
    data_rvs=scipy.stats.uniform(-1, 1).rvs,
)
matrix = matrix + matrix.T
matrix.setdiag(0)

# Looks like, at least for this problem, SM (smallest k eigenvalues) requires
# fairly permissive tolerance value compared to LM.
vals, vecs = scipy.sparse.linalg.eigsh(matrix, tol=1e-3, which="SM")
print(vals)
