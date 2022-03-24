import bisect


def main():
    series = StepSeries()
    series.add(0.3, 0)
    series.add(5.0, 2)
    series.add(4.0, 1)
    series.add(7.0, 3)
    print(series.sample(1.1, start=0.5, end=7.2))


class StepSeries:
    """
    Class for storing series of samples in non-uniform time intervals. Each
    sample, a pair (t, v) of time t and value v, is called a step. The class
    assumes an underlying step-wise function f(t), and each step (t, v) is the
    point where the function changes its value f(t) = v.
    """
    _times: list
    _values: list

    def __init__(self):
        self._times = []
        self._values = []

    def add(self, t, value):
        """
        Insert a step to the series.
        """
        end = self.end
        if end is not None and t > end:
            self._append(t, value)
        else:
            self._insert(t, value)

    def _append(self, t, value):
        self._times.append(t)
        self._values.append(value)

    def _insert(self, t, value):
        pos = bisect.bisect_left(self._times, t)
        self._times.insert(pos, t)
        self._values.insert(pos, value)

    def __len__(self):
        """
        Return the number of steps stored in the series.
        """
        return len(self._times)

    def __iter__(self):
        """
        Return an iterator that scans the stored steps in the increasing order
        in time.
        """
        return zip(self._times, self._values)

    @property
    def start(self):
        """
        Return the time of the first step, or None if no step is stored.
        """
        return self._times[0] if self._times else None

    @property
    def end(self):
        """
        Return the time of the last step, or None if no step is stored.
        """
        return self._times[-1] if self._times else None

    def at(self, t):
        """
        Return the value of the step that covers time t. There must be at least
        one step in the series.
        """
        return self._values[max(0, self._find_step(t))]

    def sample(self, dt, start=None, end=None):
        """
        Return a list of (time, value) pairs that is sampled from the stored
        steps in uniform time interval. There must be at least one step in the
        series.
        """
        if start is None:
            start = self.start
        if end is None:
            end = self.end
        step = max(0, self._find_step(start))
        t = start
        samples = []
        while t < end:
            samples.append((t, self._values[step]))
            t += dt
            step = self._next_step(step, t)
        return samples

    def _find_step(self, t):
        return bisect.bisect_right(self._times, t) - 1

    def _next_step(self, step, t):
        for next_step in range(step + 1, len(self._times)):
            if t < self._times[next_step]:
                break
            step = next_step
        return step


if __name__ == "__main__":
    main()
