#include <algorithm>
#include <cmath>
#include <cstddef>
#include <cstdint>
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
        std::size_t   grid_size          = 0;
        double        interval           = 0;
        double        diffusion_constant = 0;
        int           initial_load       = 0;
        std::uint64_t random_seed        = 0;
    };

    explicit simulation(config_type const& config);

    void step(double dt);
    void print();

private:
    void propose_delta(double dt);
    bool validate_delta() const;
    void apply_delta();

    std::size_t index(int coord) const;

private:
    config_type            _config;
    std::vector<cell_data> _grid;
    std::mt19937_64        _random;
};


int main()
{
    double const timestep = 0.1;
    long   const steps    = 50;

    simulation sim({
        .grid_size          = 25,
        .interval           = 1.0 / 25,
        .diffusion_constant = 1e-3,
        .initial_load       = 50,
        .random_seed        = 0,
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
    _grid[_grid.size() / 2].particles = config.initial_load;
}


/** Simulates time evolution of the field in given time interval. */
void simulation::step(double dt)
{
    propose_delta(dt);

    if (validate_delta()) {
        apply_delta();
    } else {
        // Reject proposed delta if it's inconsistent, and retry with finer
        // time steps.
        std::clog << "@ REJECTED!\n";
        step(dt / 2);
        step(dt / 2);
    }
}


/** Generates kinetic change of each cell and stores into the grid as `delta`. */
void simulation::propose_delta(double dt)
{
    for (auto& cell : _grid) {
        cell.delta = 0;
    }

    auto const dx = _config.interval;
    auto const grid_size = int(_grid.size());
    auto const diffusion_prob = _config.diffusion_constant * dt / (dx * dx);

    auto const binomial = [&](int trials, double prob) {
        return std::binomial_distribution<int>{trials, prob}(_random);
    };

    for (int x = 0; x < grid_size; x++) {
        auto& cell = _grid[index(x)];

        if (cell.particles == 0) {
            continue;
        }

        auto const forw_count = binomial(cell.particles, diffusion_prob);
        auto const back_count = binomial(cell.particles, diffusion_prob);

        cell.delta -= forw_count + back_count;
        _grid[index(x - 1)].delta += back_count;
        _grid[index(x + 1)].delta += forw_count;
    }
}


/** Checks if the delta proposal stored in the grid is feasible or not. */
bool simulation::validate_delta() const
{
    for (auto const& cell : _grid) {
        if (cell.particles + cell.delta < 0) {
            return false;
        }
    }
    return true;
}


/** Updates state using delta proposal stored in the grid. */
void
simulation::apply_delta()
{
    for (auto& cell : _grid) {
        cell.particles += cell.delta;
    }
}


/** Translates lattice coordinate to the index into the grid array. */
std::size_t
simulation::index(int coord) const
{
    auto const mod = int(_grid.size());

    if (0 <= coord && coord < mod) [[likely]] {
        return std::size_t(coord);
    } else {
        return std::size_t((coord < 0 ? mod + coord % mod : coord) % mod);
    }
}


/** Prints current state to stdout. */
void
simulation::print()
{
    for (auto const& cell : _grid) {
        std::cout << cell.particles << ' ';
    }
    std::cout << '\n';
}
