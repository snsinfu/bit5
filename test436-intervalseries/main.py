import bisect


def main():
    series = IntervalSeries()
    series.add(0, dt=1)
    series.add(42, dt=10)
    assert series.sample(2.6) == [0, 42, 42, 42, 42]


class IntervalSeries:
    _end: float = 0.0
    _values: list = []
    _ends: list = []

    def add(self, value, dt):
        self._end = self._end + dt
        self._values.append(value)
        self._ends.append(self._end)

    def sample(self, dt, start=None, end=None) -> list:
        start = start or 0.0
        end = end or self._end
        n_samples = int((end - start) / dt) + 1
        samples = [None] * n_samples
        segment = self._find_segment(start)
        i = 0
        t = start
        while t < end:
            samples[i] = self._values[segment]
            i += 1
            t += dt
            segment = self._next_segment(segment, t)
            if segment == len(self._values):
                break
        return samples

    def _find_segment(self, t) -> int:
        return bisect.bisect_right(self._ends, t)

    def _next_segment(self, segment, t) -> int:
        while segment < len(self._ends):
            if t < self._ends[segment]:
                break
            segment += 1
        return segment


if __name__ == "__main__":
    main()
