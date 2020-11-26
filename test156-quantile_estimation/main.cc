// https://aakinshin.net/posts/p2-quantile-estimator/
//
// FIXME: Not working correctly...
//

#include <algorithm>
#include <cstddef>
#include <iostream>
#include <random>
#include <stdexcept>


using std::size_t;

class p2_quantile_estimator
{
    static constexpr size_t marker_count = 5;

public:
    explicit p2_quantile_estimator(double probability)
        : _probability{probability}
    {
    }

    double estimate() const
    {
        if (_sample_count < marker_count) {
            // Actually one sample should be enough. This is for simplicity.
            throw std::runtime_error("insufficient data");
        }
        return _marker_heights[2];
    }

    void update(double value)
    {
        _sample_count++;

        // XXX: This update procedure may be optimized.
        auto const last_position = double(_sample_count) - 1;
        _marker_desired_positions[0] = 0;
        _marker_desired_positions[1] = last_position * _probability / 2;
        _marker_desired_positions[2] = last_position * _probability;
        _marker_desired_positions[3] = last_position * (1 + _probability) / 2;
        _marker_desired_positions[4] = last_position;

        if (_sample_count <= marker_count) {
            _marker_heights[_sample_count - 1] = value;
            std::sort(_marker_heights, _marker_heights + _sample_count);
            return;
        }

        invalidate(value);
    }

private:
    void invalidate(double value)
    {
        size_t tier;

        if (value < _marker_heights[0]) {
            // Invalidate the minimum.
            _marker_heights[0] = value;
            tier = 0;
        } else if (value < _marker_heights[1]) {
            tier = 0;
        } else if (value < _marker_heights[2]) {
            tier = 1;
        } else if (value < _marker_heights[3]) {
            tier = 2;
        } else if (value < _marker_heights[4]) {
            tier = 3;
        } else {
            // Invalidate the maximum.
            _marker_heights[4] = value;
            tier = 3; // Note: 3, not 4
        }

        for (size_t i = tier + 1; i < marker_count; i++) {
            _marker_positions[i]++;
        }

        for (size_t i = 1; i < marker_count - 1; i++) {
            double const error = _marker_desired_positions[i] - double(_marker_positions[i]);

            if ((error >= 1 && _marker_positions[i + 1] - _marker_positions[i] > 1) ||
                (error <= -1 && _marker_positions[i] - _marker_positions[i - 1] > 1)) {

                auto const direction = (error < 0) ? -1 : 1;
                auto const estimated_height = parabolic(i, direction);

                if (estimated_height > _marker_heights[i - 1] &&
                    estimated_height < _marker_heights[i + 1]) {
                    _marker_heights[i] = estimated_height;
                } else {
                    _marker_heights[i] = linear(i, direction);
                }

                _marker_positions[i]++;
            }
        }
    }

    double parabolic(size_t tier, double direction) const
    {
        double const q = _marker_heights[tier];
        double const q_lower = _marker_heights[tier - 1];
        double const q_upper = _marker_heights[tier + 1];

        double const n = double(_marker_positions[tier]);
        double const n_lower = double(_marker_positions[tier - 1]);
        double const n_upper = double(_marker_positions[tier + 1]);

        return q + direction / (n_upper - n_lower) * (
                (n - n_lower + direction) * (q_upper - q) / (n_upper - n)
                +
                (n_upper - n - direction) * (q - q_lower) / (n - n_lower)
            );
    }

    double linear(size_t tier, double direction) const
    {
        double const q = _marker_heights[tier];
        double const q_lower = _marker_heights[tier - 1];
        double const q_upper = _marker_heights[tier + 1];

        double const n = double(_marker_positions[tier]);
        double const n_lower = double(_marker_positions[tier - 1]);
        double const n_upper = double(_marker_positions[tier + 1]);

        if (direction < 0) {
            return q - (q_lower - q) / (n_lower - n);
        } else {
            return q + (q_upper - q) / (n_upper - n);
        }
    }

private:
    double _probability;
    size_t _sample_count = 0;
    double _marker_heights[marker_count] = {};
    size_t _marker_positions[marker_count] = {};
    double _marker_desired_positions[marker_count] = {};
};


int main()
{
    p2_quantile_estimator median(0.5);

    std::mt19937_64 random;
    std::uniform_real_distribution<double> distr;

    for (int i = 0; i < 10000; i++) {
        median.update(distr(random));
    }

    std::cout << median.estimate() << '\n';
}
