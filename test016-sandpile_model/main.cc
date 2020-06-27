#include <chrono>
#include <cstddef>
#include <iostream>
#include <string>
#include <vector>

#include "buffer.hpp"
#include "matrix.hpp"


class sandpile
{
    struct point_change {
        int x;
        int y;
        int delta;
    };

public:
    explicit sandpile(int size)
        : _size{size}
        , _vertices{size, size}
    {
    }

    int size() const
    {
        return _size;
    }

    matrix<int> const& vertices() const
    {
        return _vertices;
    }

    void fill(int value)
    {
        for (int row = 0; row < _size; row++) {
            for (int col = 0; col < _size; col++) {
                _vertices(row, col) = value;
            }
        }
    }

    void increment(int x, int y)
    {
        _vertices(x, y)++;
    }

    int tick()
    {
        int const threshold = 4;
        int topples = 0;

        _changeset.clear();

        for (int x = 0; x < _size; x++) {
            for (int y = 0; y < _size; y++) {
                if (_vertices(x, y) >= threshold) {
                    _changeset.push_back({x, y, -threshold});
                    topples++;

                    if (x + 1 < _size) {
                        _changeset.push_back({x + 1, y, 1});
                    }
                    if (x > 0) {
                        _changeset.push_back({x - 1, y, 1});
                    }
                    if (y + 1 < _size) {
                        _changeset.push_back({x, y + 1, 1});
                    }
                    if (y > 0) {
                        _changeset.push_back({x, y - 1, 1});
                    }
                }
            }
        }

        for (auto [x, y, delta] : _changeset) {
            _vertices(x, y) += delta;
        }

        return topples;
    }

private:
    int                  _size;
    matrix<int>          _vertices;
    buffer<point_change> _changeset;
};


int is_uniform(sandpile const& sp, int value)
{
    auto const& vertices = sp.vertices();
    for (int row = 0; row < vertices.rows(); row++) {
        for (int col = 0; col < vertices.cols(); col++) {
            if (vertices(row, col) != value) {
                return false;
            }
        }
    }
    return true;
}


int main(int argc, char** argv)
{
    if (argc != 3) {
        std::cerr << "usage: main <size> <init>\n";
        return 1;
    }
    int const size = std::stoi(argv[1]);
    int const init = std::stoi(argv[2]);

    sandpile sp{size};
    sp.fill(init);

    using clock = std::chrono::steady_clock;
    auto const start = clock::now();
    auto iterations = 0L;

    for (; ; iterations++) {
        if (iterations != 0 && is_uniform(sp, init)) {
            std::cout << "Period: " << iterations << '\n';
            break;
        }

        auto const center = sp.size() / 2;
        sp.increment(center, center);
#ifdef AVALANCHE
        while (sp.tick() != 0) {
            continue;
        }
#else
        sp.tick();
#endif
    }

    auto const end = clock::now();
    auto const dur = std::chrono::duration<double>(end - start).count();

    std::cout << dur / double(iterations) * 1e9 << " ns/iter\n";
}
