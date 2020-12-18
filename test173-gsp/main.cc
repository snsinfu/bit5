#include <cstdint>
#include <iostream>
#include <random>

#include "gsp.hpp"


int main()
{
    long const simulation_steps = 10'000;
    std::uint64_t const random_seed = 0;
    std::size_t const species_count = 30;

    std::mt19937_64 random(random_seed);

    gsp::state init_state;
    init_state.species.resize(species_count);
    {
        std::uniform_int_distribution<int> count(0, 10);
        std::generate(init_state.species.begin(), init_state.species.end(), [&] {
            return count(random);
        });
    }

    gsp::simulation sim(init_state);

    for (std::size_t i = 0; i < species_count; i++) {
        std::uniform_int_distribution<std::size_t> species(0, species_count);
        std::uniform_real_distribution<double> rate(0.1, 1.0);

        auto const a = i;
        auto const b = species(random);

        sim.add_reaction(
            gsp::simple_transformation({
                .reactant = a,
                .product = b,
                .rate = rate(random),
            })
        );

        sim.add_reaction(
            gsp::simple_transformation({
                .reactant = b,
                .product = a,
                .rate = rate(random),
            })
        );
    }

    for (long step = 1; step <= simulation_steps; step++) {
        auto const reaction_id = sim.step(random);
        std::cout << sim.state() << '\n';
    }
}
