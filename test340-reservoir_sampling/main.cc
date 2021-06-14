#include <cstddef>
#include <iostream>
#include <random>
#include <vector>


int main()
{
    constexpr std::size_t sample_count = 10;
    constexpr std::size_t stream_size = 1000;

    std::mt19937_64 random;
    {
        std::random_device source;
        std::seed_seq seed_seq{source(), source(), source(), source()};
        random.seed(seed_seq);
    }

    std::vector<int> reservoir;
    std::size_t items_seen = 0;

    // Now we uniformly sample some values from a stream of numbers. Here we
    // use the sequence (0,1,2,3,4,...) as the input stream.

    // Just store the first values.
    for (; items_seen < sample_count; items_seen++) {
        auto const item = int(items_seen);
        reservoir.push_back(item);
    }

    // Sample from the stream...
    for (; items_seen < stream_size; items_seen++) {
        auto const item = int(items_seen);

        std::uniform_int_distribution<std::size_t> distr(0, items_seen - 1);
        auto const pos = distr(random);
        if (pos < sample_count) {
            reservoir[pos] = item;
        }
    }

    // Inspect the result.
    for (auto const& sample : reservoir) {
        std::cout << sample << ' ';
    }
    std::cout << '\n';
}
