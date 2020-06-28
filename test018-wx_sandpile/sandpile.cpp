#include "sandpile.h"

#include <random>


Sandpile::Sandpile(int width, int height)
    : _vertices(width, height)
{
}

int
Sandpile::tick()
{
    auto const topples = updateChangeset();
    for (auto [x, y, delta] : _changeset) {
        _vertices(y, x) += delta;
    }
    return topples;
}

int
Sandpile::updateChangeset()
{
    constexpr int threshold = 4;

    auto const width = _vertices.cols();
    auto const height = _vertices.rows();

    int topples = 0;
    _changeset.clear();

    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            if (_vertices(y, x) < threshold) {
                continue;
            }

            topples++;
            _changeset.push_back({x, y, -threshold});

            if (x > 0) {
                _changeset.push_back({x - 1, y, 1});
            }
            if (y > 0) {
                _changeset.push_back({x, y - 1, 1});
            }
            if (x < width - 1) {
                _changeset.push_back({x + 1, y, 1});
            }
            if (y < height - 1) {
                _changeset.push_back({x, y + 1, 1});
            }
        }
    }

    return topples;
}

void randomize(Sandpile& sandpile)
{
    std::mt19937 random;
    std::random_device source;
    std::seed_seq seed = {source(), source(), source(), source()};
    random.seed(seed);

    std::uniform_int_distribution<int> uniform{0, 5};

    for (int x = 0; x < sandpile.width(); x++) {
        for (int y = 0; y < sandpile.height(); y++) {
            sandpile(x, y) = uniform(random);
        }
    }
}

void initialize(Sandpile& sandpile, int state)
{
    for (int x = 0; x < sandpile.width(); x++) {
        for (int y = 0; y < sandpile.height(); y++) {
            sandpile(x, y) = state;
        }
    }
}
