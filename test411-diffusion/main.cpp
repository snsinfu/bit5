#include <algorithm>
#include <cmath>
#include <cstddef>
#include <cstdint>
#include <cstdlib>
#include <iostream>
#include <iterator>
#include <numeric>
#include <random>
#include <vector>


struct cell_data
{
    int particles = 0;
    int delta     = 0;
};


class simulation
{
public:
    struct config_type
    {
        std::size_t   grid_size       = 0;
        double        diffusion_rate  = 0;
        double        initial_density = 0;
        std::uint64_t random_seed     = 0;
    };

    explicit simulation(config_type const& config);

    void initialize();
    void step(double dt);
    void print();

private:
    void step_deltas(double dt);
    void step_apply(double dt);

    std::size_t index(int coord) const;
    int         poisson(double mean);

private:
    config_type            _config;
    std::vector<cell_data> _grid;
    std::mt19937_64        _random;
};


int main()
{
    double const timestep = 0.01;
    long   const steps    = 100;

    simulation sim({
        .grid_size       = 32,
        .diffusion_rate  = 10.0,
        .initial_density = 0.5,
        .random_seed     = 0,
    });

    sim.print();

    for (long step = 1; step <= steps; step++) {
        sim.step(timestep);
        sim.print();
    }
}


simulation::simulation(config_type const& config)
: _config(config)
, _grid(config.grid_size)
, _random(config.random_seed)
{
    initialize();
}


void simulation::initialize()
{
    std::generate(_grid.begin(), _grid.end(), [&] {
        return cell_data { .particles = poisson(_config.initial_density) };
    });
}


void simulation::step(double dt)
{
    step_deltas(dt);
    step_apply(dt);
}


void simulation::step_deltas(double dt)
{
    for (auto& cell : _grid) {
        cell.delta = 0;
    }

    auto const min_coord = 0;
    auto const max_coord = int(_grid.size()) - 1;

    for (int x = min_coord; x <= max_coord; x++) {
        auto& cell = _grid[index(x)];

        if (cell.particles == 0) {
            continue;
        }

        auto const forw_count = poisson(_config.diffusion_rate * dt);
        auto const back_count = poisson(_config.diffusion_rate * dt);

        cell.delta -= forw_count + back_count;
        _grid[index(x - 1)].delta += back_count;
        _grid[index(x + 1)].delta += forw_count;
    }
}


void simulation::step_apply(double dt)
{
    for (auto const& cell : _grid) {
        if (cell.particles + cell.delta < 0) {
            // Reject this step and retry with smaller timesteps.
            std::clog << "@ REJECTED!\n";
            step(dt / 2);
            step(dt / 2);
            return;
        }
    }

    for (auto& cell : _grid) {
        cell.particles += cell.delta;
    }
}


std::size_t simulation::index(int coord) const
{
    auto const mod = int(_grid.size());

    if (0 <= coord && coord < mod) [[likely]] {
        return std::size_t(coord);
    } else {
        return std::size_t(std::abs(coord % mod));
    }
}


int simulation::poisson(double mean)
{
    return std::poisson_distribution<int>{mean}(_random);
}


void simulation::print()
{
    auto const total = std::accumulate(
        _grid.begin(), _grid.end(), 0, [](int sum, cell_data const& cell) {
            return sum + cell.particles;
        }
    );
    std::cout << '[' << total << ']';

    for (auto const& cell : _grid) {
        std::cout << ' ' << cell.particles;
    }
    std::cout << '\n';
}
