import numba
import numpy as np


def main():
    random = np.random.RandomState(0)
    path = generate_path((100, 3), random)

    fourier, freqs = compute_fourier(path)
    times = np.arange(len(path))
    modes = reconstruct_modes(fourier, freqs, times)

    # Reconstruction error
    for k in range(10):
        recons_path = np.sum(modes[:k], axis=0)
        mse = np.mean((recons_path - path) ** 2) ** 0.5
        print(f"{k}\t{mse:g}")


def reconstruct_modes(fourier, freqs, times):
    """
    Reconstruct fourier modes at given time points. The shape of the returned
    array is `(mode, time, values...)`.
    """
    M = len(freqs)
    T = len(times)
    V = fourier.shape[1:]

    amps = np.abs(fourier)
    phase_0 = np.angle(fourier)
    phase_t = 2 * np.pi * freqs[:, None] * times[None, :]
    assert amps.shape == (M, ) + V
    assert phase_0.shape == (M, ) + V
    assert phase_t.shape == (M, T)

    amps = amps.reshape((M, 1) + V)
    phase_0 = phase_0.reshape((M, 1) + V)
    phase_t = phase_t.reshape((M, T) + (1, ) * len(V))
    assert len(amps.shape) == len(phase_0.shape) == len(phase_t.shape)

    modes = amps * np.cos(phase_0 + phase_t)
    assert modes.shape == (M, T) + V

    return modes


def compute_fourier(seq):
    """
    Compute the fourier coefficients of real-valued multi-dimensional sequence.
    """
    fourier = np.fft.fft(seq, axis=0)
    freqs = np.fft.fftfreq(len(seq))

    # Negative frequency modes are redundant for real-valued sequence. So remove
    # these and reorder modes in increasing order.
    order = np.argsort(freqs)
    order = order[freqs[order] >= 0]
    freqs = freqs[order]
    fourier = fourier[order]

    # Scale coefficients so that `abs(fourier)` gives the amplitude of modes.
    fourier[0] *= 1 / len(seq)
    fourier[1:] *= 2 / len(seq)

    return fourier, freqs


def generate_path(size, random, window=10):
    """
    Generate random walk path with correlated steps.
    """
    steps = random.normal(0, 1, size)
    steps = smooth(steps, window)
    return np.cumsum(steps, axis=0)


def smooth(seq, window):
    """
    Smooth sequence along the first axis. Uses "valid" boundary conditions.
    """
    out = np.empty(seq.shape, seq.dtype)
    _do_smooth(seq, out, window)
    return out


@numba.jit
def _do_smooth(seq_in, seq_out, window):
    length = len(seq_in)

    for pos_out in range(length):
        start = pos_out - window // 2
        end = pos_out + (window + 1) // 2
        if start < 0:
            start = 0
        if end > length:
            end = length

        seq_out[pos_out] = 0
        for pos_in in range(start, end):
            seq_out[pos_out] += seq_in[pos_in]
        seq_out[pos_out] *= 1 / (end - start)


main()
