#include <algorithm>
#include <cmath>
#include <cstddef>
#include <iostream>
#include <random>
#include <vector>


/** Three-dimensional point in Cartesian coordinate system. */
struct point_type
{
    double x = 0;
    double y = 0;
    double z = 0;
};


/** Reduces point in a periodic system to the canonical image. */
point_type
reduce_to_canonical(point_type const& point, double box_size)
{
    auto const k = 1 / box_size;

    return point_type {
        k * point.x - std::floor(k * point.x),
        k * point.y - std::floor(k * point.y),
        k * point.z - std::floor(k * point.z),
    };
}


/** Three-dimensional mesh grid with periodic boundary conditions. */
template<typename T>
class spatial_mesh
{
public:
    struct config_type
    {
        double      span      = 0;
        std::size_t divisions = 1;
    };


    explicit
    spatial_mesh(config_type const& config)
        : _span{config.span}
        , _divisions{config.divisions}
        , _cells(config.divisions * config.divisions * config.divisions)
    {
    }


    T*
    begin()
    {
        return _cells.data();
    }


    T*
    end()
    {
        return _cells.data() + _cells.size();
    }


    T&
    query(point_type const& point)
    {
        return _cells[locate(point)];
    }


private:
    /** Locates the index of the grid cell covering given point. */
    std::size_t
    locate(point_type const& point) const
    {
        auto const image = reduce_to_canonical(point, _span);

        auto const quantize = [=](double frac) {
            auto const level = std::size_t(int(frac * double(_divisions)));
            return std::min(level, _divisions - 1);
        };
        auto const x = quantize(image.x);
        auto const y = quantize(image.y);
        auto const z = quantize(image.z);

        return x + (y + z * _divisions) * _divisions;
    }


private:
    double         _span;
    std::size_t    _divisions;
    std::vector<T> _cells;
};


int
main()
{
    std::mt19937_64 random;

    spatial_mesh<int> mesh({
        .span      = 1.0,
        .divisions = 10,
    });

    // Distribute "things" on the mesh.
    std::poisson_distribution init{0.5};
    for (auto& cell : mesh) {
        cell = init(random);
    }

    // Random walker collects and dumps things from/to the mesh.
    double const loading_rate = 0.1;
    double const unloading_rate = 0.05;
    int const max_load = 5;

    std::normal_distribution<double> walk{0, 0.01};
    point_type pos = {0, 0, 0};
    int load = 0;

    for (long step = 0; step < 100000; step++) {
        pos.x += walk(random);
        pos.y += walk(random);
        pos.z += walk(random);

        auto& pool = mesh.query(pos);

        std::poisson_distribution<int> loading{loading_rate * double(pool)};
        std::poisson_distribution<int> unloading{unloading_rate};

        auto const min_delta = -load;
        auto const max_delta = max_load - load;
        auto const delta = std::clamp(
            loading(random) - unloading(random),
            min_delta,
            max_delta
        );

        load += delta;
        pool -= delta;

        if (step % 100 == 0) {
            std::cout << load << ' ';
        }
    }
    std::cout << '\n';
}
