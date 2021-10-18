#include <cstddef>
#include <cstdint>
#include <iostream>
#include <random>
#include <string>
#include <vector>


int main()
{
    std::mt19937_64 random;

    std::vector<int> histogram(6);

    for (int i = 0; i < 1000; i++) {
        auto const bits = random();

        // Generate approximately uniformly distributed integer in [0,6) without
        // slow modulo operation. Here, we use the lowest 8-bits of the random
        // number as a uniformly distributed fixed point real number in [0,1).
        // Multiply it by 6 to produce a fixed point real number in [0,6). Then,
        // truncate the fractional part (the lower 8 bits) to get an integer.
        auto const number = (bits & 0xFFFF) * 6 >> 16;

        histogram.at(number)++;
    }

    for (std::size_t i = 0; i < histogram.size(); i++) {
        std::cout
            << "["
            << i
            << "]: "
            << histogram.at(i)
            << '\n';
    }
}
