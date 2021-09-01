#include <algorithm>
#include <iostream>
#include <iterator>
#include <random>

#include "simulator/simulator.hpp"


int main()
{
    simulator::random_type random;
    simulator sim{10};

    sim.set_birth_rate(0.1);
    sim.set_death_rate(0.02);

    for (int step = 0; step < 20; step++) {
        sim.step(10, random);
        std::copy(sim.begin(), sim.end(), std::ostream_iterator<int>{std::cout, " "});
        std::cout << '\n';
    }
}
