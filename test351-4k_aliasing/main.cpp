#include <algorithm>
#include <chrono>
#include <cstddef>
#include <iostream>
#include <random>
#include <vector>


static void demo(std::vector<float>& data);


int main()
{
    using clock = std::chrono::high_resolution_clock;

    constexpr std::size_t data_size = 1'000'000;
    constexpr std::size_t repeat = 100;

    std::vector<float> data(data_size);

    std::mt19937_64 random;
    std::normal_distribution<float> normal;
    std::generate(data.begin(), data.end(), [&] { return normal(random); });

    auto const start_time = clock::now();
    for (std::size_t i = 0; i < repeat; i++) {
        demo(data);
    }
    auto const end_time = clock::now();
    auto const elapsed_time = std::chrono::duration<double>(end_time - start_time);
    auto const throughput = double(repeat) / elapsed_time.count();

    std::cout << throughput << '\n';
}


void demo(std::vector<float>& data)
{
    constexpr std::size_t skip = 1008;
    // constexpr std::size_t skip = 4096;
    // constexpr std::size_t skip = 4112;

    for (std::size_t write_pos = skip; write_pos + 16 < data.size(); write_pos++) {
        std::size_t const start = write_pos - skip;
        std::size_t const end = start + 16;

        float sum = 0;
        for (std::size_t read_pos = start; read_pos < end; read_pos++) {
            sum += data[read_pos];
        }

        for (std::size_t i = 0; i < 16; i++) {
            data[write_pos + i] += sum;
        }
    }
}
