#include <cassert>
#include <iostream>
#include <random>
#include <vector>


constexpr std::size_t num_data = 10'000'000;
constexpr int bin_width = 100;
constexpr int max_number = 9999;


std::vector<int>
make_data()
{
    std::mt19937_64 random(0);
    std::vector<int> data;

    std::uniform_int_distribution<int> uniform(0, max_number + 1);
    data.reserve(num_data);
    for (std::size_t i = 0; i < num_data; i++) {
        data.push_back(uniform(random));
    }

    return data;
}


int
main()
{
    auto data = make_data();

    auto const num_bins = std::size_t((max_number + bin_width) / bin_width);
    auto const mean_density = double(num_data) / double(num_bins);
    auto const bin_depth = std::size_t(mean_density * 2);
    std::vector<int> bins(num_bins * bin_depth, -1);

    for (auto value : data) {
        auto const bin_index = std::size_t(value / bin_width);
        auto index = bin_index * bin_depth;
        while (bins[index] != -1) {
            index++;
        }
        assert(index < (bin_index + 1) * bin_depth);
        bins[index] = value;
    }
}
