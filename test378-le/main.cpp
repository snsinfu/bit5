#include <algorithm>
#include <cstddef>
#include <iostream>
#include <vector>


struct chain_config
{
    std::size_t              length;
    std::vector<std::size_t> boundaries;
    std::vector<std::size_t> positions;
};


int main()
{
    chain_config const config = {
        .length     = 301,
        .boundaries = {0, 100, 200, 300},
        .positions  = {50, 150, 250},
    };

    for (auto const& pos : config.positions) {
        auto const end_it = std::lower_bound(
            config.boundaries.begin(),
            config.boundaries.end(),
            pos
        );
        auto const start = *(end_it - 1);
        auto const end = *end_it;

        std::cout
            << pos
            << '\t'
            << start << ':' << end
            << '\n';
    }
}
