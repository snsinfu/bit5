#include <algorithm>
#include <cassert>
#include <chrono>
#include <iostream>
#include <random>
#include <vector>


constexpr std::size_t num_data = 1'000'000;
constexpr int bin_width = 10;
constexpr int max_number = 9999;


std::vector<int>
make_data()
{
    std::mt19937_64 random(0);
    std::vector<int> data;

    std::uniform_int_distribution<int> uniform(0, max_number);
    data.reserve(num_data);
    for (std::size_t i = 0; i < num_data; i++) {
        data.push_back(uniform(random));
    }

    return data;
}


void
insertion_sort(std::vector<int>& data)
{
    for (std::size_t i = 1; i < data.size(); i++) {
        for (std::size_t j = i; j > 0; j--) {
            if (data[j - 1] <= data[j]) {
                break;
            }
            std::swap(data[j], data[j - 1]);
        }
    }
}


void
bin_sort(std::vector<int>& data)
{
    auto const num_bins = std::size_t((max_number + bin_width) / bin_width);
    auto const mean_density = double(num_data) / double(num_bins);
    auto const bin_depth = std::size_t(mean_density * 2) + 1;
    std::vector<std::size_t> bin_sizes(num_bins);
    std::vector<int> bins(num_bins * bin_depth);

    for (auto value : data) {
        auto const bin_index = std::size_t(value / bin_width);
        auto const index = bin_index * bin_depth + bin_sizes[bin_index];
        bin_sizes[bin_index]++;
        bins[index] = value;
    }

    data.clear();

    for (std::size_t bin_index = 0; bin_index < num_bins; bin_index++) {
        auto const start = bin_index * bin_depth;
        auto const end = start + bin_sizes[bin_index];
        for (std::size_t i = start; i < end; i++) {
            data.push_back(bins[i]);
        }
    }

    insertion_sort(data);
}


int
main()
{
    auto data = make_data();

    using clock = std::chrono::steady_clock;
    auto const start_time = clock::now();
    bin_sort(data);
    // insertion_sort(data);
    auto const end_time = clock::now();
    auto const duration = std::chrono::duration<double>(end_time - start_time).count();

    assert(std::is_sorted(data.begin(), data.end()));
    std::cout << "Sort finished in: " << duration << " sec.\n";
}
