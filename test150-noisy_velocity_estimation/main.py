import numpy as np

num_steps = 1000
random = np.random.RandomState(0)

z = np.cumsum(random.uniform(0, 0.01, size=num_steps))
angle = np.cumsum(random.uniform(0, 0.01, size=num_steps))
x = np.cos(angle)
y = np.sin(angle)
path = np.transpose([x, y, z])

time = np.arange(len(path))

shifted_time = time - time.mean()
shifted_path = path - path.mean(0)
velocity = (shifted_time[:, None] * shifted_path).sum(0) / (shifted_time ** 2).sum()

print(velocity)
