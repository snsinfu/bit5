import numpy as np


def main():
    dimension = 10
    n_samples = 10000
    random = np.random.RandomState(0)

    target = make_unit_vector(dimension)
    directions = make_random_directions(random, n_samples, dimension)
    projections = directions.dot(target)[:, None] * directions
    mean_projection = projections.mean(axis=0)

    result = dimension * mean_projection
    print(format_vector(result))


def make_unit_vector(dimension):
    target = np.zeros(dimension)
    target[0] = 1
    return target


def make_random_directions(random, count, dimension):
    normals = random.normal(size=(count, dimension))
    lengths = np.linalg.norm(normals, axis=1)
    return normals / lengths[:, None]


def format_vector(vector):
    return " ".join(f"{x:.4f}" for x in vector)


main()
