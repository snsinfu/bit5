import numpy as np


def main():
    random = np.random.RandomState(0)
    values = random.uniform(-10, 10, size=1000)

    logscale_resolution = estimate_logscale_resolution(values)
    percentile_resolution = estimate_percentile_resolution(values)

    logscale_factor = compute_scaleoffset_factor(logscale_resolution)
    percentile_factor = compute_scaleoffset_factor(percentile_resolution)

    logscale_error = compute_scaleoffset_error(values, logscale_resolution)
    percentile_error = compute_scaleoffset_error(values, percentile_resolution)

    logscale_int = int(values.max() / logscale_resolution)
    percentile_int = int(values.max() / percentile_resolution)

    print("Logscale:")
    print(f"  resolution  {logscale_resolution:g}")
    print(f"  factor      {logscale_factor:g}")
    print(f"  max error   {logscale_error:g}")
    print(f"  max int     {logscale_int:g}")
    print("")
    print("Percentile:")
    print(f"  resolution  {percentile_resolution:g}")
    print(f"  factor      {percentile_factor:g}")
    print(f"  max error   {percentile_error:g}")
    print(f"  max int     {percentile_int:g}")


def compute_scaleoffset_factor(resolution):
    return -int(np.log10(resolution))


def compute_scaleoffset_error(values, resolution):
    scaleoffset_values = np.round(values / resolution) * resolution
    errors = np.abs(scaleoffset_values - values)
    max_error = errors.max()
    return max_error


def estimate_logscale_resolution(values):
    magnitudes = np.abs(values)
    log_magnitudes = np.log(magnitudes[magnitudes > 0])
    m = log_magnitudes.mean()
    s = log_magnitudes.std()
    resolution = np.exp(m - 3 * s) / 10
    return round_down(resolution)


def estimate_percentile_resolution(values, q=1):
    magnitudes = np.abs(values)
    resolution = np.percentile(magnitudes, q=q) / 10
    return round_down(resolution)


def round_down(x):
    return 10 ** np.floor(np.log10(x))


main()
