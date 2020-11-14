import numpy as np


num_steps = 1000
random = np.random.RandomState(0)

x = np.linspace(0, 1, num=num_steps)
y = np.linspace(0, 1, num=num_steps)

x += random.normal(0, 0.1, size=x.shape)
y += random.normal(0, 0.1, size=y.shape)


def compute_correlation(u, v):
    return np.mean(u * v) / np.sqrt(np.mean(u**2) * np.mean(v**2))


def smooth(signal, window):
    lpad = window // 2
    rpad = window - lpad - 1
    padded_signal = np.pad(signal, pad_width=((lpad, rpad)), mode="reflect")
    kernel = np.exp(-np.linspace(-3, 3, num=window)**2 / 2)
    kernel = kernel / kernel.sum()
    return np.convolve(padded_signal, kernel, mode="valid")


# Instantaneous velocity correlation
u = x[1:] - x[:-1]
v = y[1:] - y[:-1]
instantaneous_correlation = compute_correlation(u, v)

# Smoothed instantaneous velocity correlation
u = smooth(x[1:] - x[:-1], window=100)
v = smooth(y[1:] - y[:-1], window=100)
smooth_instantaneous_correlation = compute_correlation(u, v)

# Long-term displacement correlation
u = x[100:] - x[:-100]
v = y[100:] - y[:-100]
longterm_correlation = compute_correlation(u, v)

# Long-term displacement correlation
u = smooth(x[100:] - x[:-100], window=25)
v = smooth(y[100:] - y[:-100], window=25)
smooth_longterm_correlation = compute_correlation(u, v)

print(instantaneous_correlation)
print(smooth_instantaneous_correlation)
print(longterm_correlation)
print(smooth_longterm_correlation)
