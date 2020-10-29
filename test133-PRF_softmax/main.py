import numpy as np


def softmax(x, y):
    return np.exp(np.dot(x, y))


def prf(x, y, random):
    n = len(x)
    omega = random.normal(size=n)
    ax = np.exp(np.dot(omega, x) - np.dot(x, x) / 2)
    ay = np.exp(np.dot(omega, y) - np.dot(y, y) / 2)
    return ax * ay


random = np.random.RandomState(0)

for _ in range(20):
    n = 5

    x = random.uniform(-1, 1, size=n) / np.sqrt(n)
    y = random.uniform(-1, 1, size=n) / np.sqrt(n)

    u = softmax(x, y)
    v = np.mean([prf(x, y, random) for _ in range(100)])

    print(f"{u:g}\t{v:g}")
