#include <cstddef>
#include <random>

#include "simulator.hpp"


simulator::simulator(std::size_t cells)
    : _cells_state(cells)
{
}


int const*
simulator::begin() const
{
    return _cells_state.data();
}


int const*
simulator::end() const
{
    return _cells_state.data() + _cells_state.size();
}


std::size_t
simulator::size() const
{
    return std::size_t(end() - begin());
}


void
simulator::set_birth_rate(double rate)
{
    _birth_rate = rate;
}


void
simulator::set_death_rate(double rate)
{
    _death_rate = rate;
}


void
simulator::step(double dt, random_type& random)
{
    auto poisson = [&](double rate) {
        return std::poisson_distribution<int>{rate * dt}(random);
    };

    for (auto& cell : _cells_state) {
        cell += poisson(_birth_rate);
        cell -= poisson(_death_rate * double(cell));

        if (cell < 0) {
            cell = 0;
        }
    }
}
