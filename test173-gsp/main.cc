#include <cstdint>
#include <iostream>
#include <random>

#include "gsp.hpp"


int main()
{
    long const simulation_steps = 10'000;
    std::uint64_t const random_seed = 0;

    gsp::simulation sim({.species = {0, 0, 0}});

    // (nothing) --> A --> B --> C --> (nothing)
    sim.add_reaction(gsp::simple_generation({.species = 0, .rate = 0.1}));
    sim.add_reaction(gsp::simple_transformation({.reactant = 0, .product = 1, .rate = 0.1}));
    sim.add_reaction(gsp::simple_transformation({.reactant = 1, .product = 2, .rate = 0.1}));
    sim.add_reaction(gsp::simple_decay({.species = 2, .rate = 0.1}));

    std::cout << sim.state() << '\n';

    std::mt19937_64 random(random_seed);

    for (long step = 1; step <= simulation_steps; step++) {
        auto const reaction_id = sim.step(random);
        std::cout << sim.state() << '\n';
    }
}
