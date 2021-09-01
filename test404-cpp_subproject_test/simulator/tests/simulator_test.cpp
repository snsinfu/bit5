#include <doctest/doctest.h>

#include "../simulator.hpp"


TEST_CASE("simulator::size - returns the number of cells") {
    simulator sim{42};
    CHECK(sim.size() == 42);
    CHECK(sim.end() == sim.begin() + 42);
}
