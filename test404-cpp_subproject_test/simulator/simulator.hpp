#pragma once

#include <cstddef>
#include <random>
#include <vector>


class simulator
{
public:
    using random_type = std::mt19937_64;

    explicit    simulator(std::size_t cells);
    int const*  begin() const;
    int const*  end() const;
    std::size_t size() const;
    void        set_birth_rate(double rate);
    void        set_death_rate(double rate);
    void        step(double dt, random_type& random);

private:
    double           _birth_rate = 0;
    double           _death_rate = 0;
    std::vector<int> _cells_state;
};
