// B(off)    --> B(on) + a
// B(on) + a --> B(off)
// ∅         --> b          (mediated by B(on))
// b         --> ∅

#include <cstdint>
#include <iostream>
#include <random>

#include "gsp.hpp"
#include "reactions.hpp"


int main()
{
    const long simulation_steps = 1000;
    const std::uint64_t random_seed = 0;

    gsp::state const init_state = {
        .species = {0, 1, 0, 0, 1, 0}
    };

    gsp::simulation sim(init_state);

    // A(off) --> A(on) + b
    sim.add_reaction(gsp::simple_dissociation({
        .reactant = 0,
        .product1 = 1,
        .product2 = 5,
        .rate     = 1,
    }));

    // A(on) + b --> A(off)
    sim.add_reaction(gsp::simple_association({
        .reactant1 = 1,
        .reactant2 = 5,
        .product   = 0,
        .rate      = 1,
    }));

    // ∅ --> a  (mediated by A(on))
    sim.add_reaction(gsp::linear_mediated_generation({
        .species  = 2,
        .rate     = 1,
        .mediator = 1,
    }));

    // a --> ∅
    sim.add_reaction(gsp::simple_decay({
        .species  = 2,
        .rate     = 1,
    }));

    // B(off) --> B(on) + a
    sim.add_reaction(gsp::simple_dissociation({
        .reactant = 3,
        .product1 = 4,
        .product2 = 2,
        .rate     = 1,
    }));

    // B(on) + a --> B(off)
    sim.add_reaction(gsp::simple_association({
        .reactant1 = 4,
        .reactant2 = 2,
        .product   = 3,
        .rate      = 1,
    }));

    // ∅ --> b  (mediated by B(on))
    sim.add_reaction(gsp::linear_mediated_generation({
        .species  = 5,
        .rate     = 1,
        .mediator = 4,
    }));

    // b --> ∅
    sim.add_reaction(gsp::simple_decay({
        .species  = 5,
        .rate     = 1,
    }));

    std::mt19937_64 random(random_seed);

    std::cout << "time A_off A_on A B_off B_on B\n";
    std::cout << sim.state() << '\n';

    for (long step = 1; step <= simulation_steps; step++) {
        sim.step(random);

        std::cout << sim.state() << '\n';
    }
}
