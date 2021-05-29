// Glauber dynamics of a 2D Ising system.

#include <algorithm>
#include <cmath>
#include <cstddef>
#include <cstdint>
#include <iostream>
#include <numeric>
#include <random>
#include <string>
#include <vector>

#include <h5.hpp>


struct simulation_config
{
    std::size_t   size              = 100;
    double        interaction       = 1;
    double        external_field    = 0;
    double        temperature       = 1;
    std::int64_t  sampling_steps    = 1000;
    std::int64_t  sampling_interval = 1;
    std::int64_t  logging_interval  = 100;
    std::uint64_t random_seed       = 0;
    std::string   output_filename;
};

static void run_simulation(simulation_config const& config);


int main()
{
    simulation_config const config = {
        .size              = 200,
        .temperature       = 1,
        .sampling_steps    = 20'000'000,
        .sampling_interval = 200'000,
        .logging_interval  = 200'000,
        .random_seed       = 0,
        .output_filename = "_output.h5",
    };
    run_simulation(config);
}


void run_simulation(simulation_config const& config)
{
    std::size_t const num_spins = config.size * config.size;

    h5::file output{config.output_filename, "w"};
    std::vector<std::int8_t> spins(num_spins);
    std::mt19937_64 random;

    // Seed
    {
        std::seed_seq seed_seq{config.random_seed};
        random.seed(seed_seq);
    }

    // Initialization
    {
        std::uniform_int_distribution<std::int8_t> binary{0, 1};
        std::generate(spins.begin(), spins.end(), [&] {
            return std::int8_t(-1 + 2 * binary(random));
        });
    }

    // Simulation handler
    auto snapshots_dataset = output.dataset<h5::i8, 3>("snapshots");
    auto snapshots_stream = snapshots_dataset.stream_writer(
        {config.size, config.size},
        {.compression = 1, .scaleoffset = 0}
    );

    auto compute_magnetization = [&] {
        double magnetization = 0;
        for (auto const& spin : spins) {
            magnetization += spin;
        }
        return magnetization / num_spins;
    };

    auto print_progress = [&](std::int64_t step) {
        std::clog
            << step
            << '\t'
            << compute_magnetization()
            << '\n';
    };

    auto save_snapshot = [&] {
        snapshots_stream.write(spins.data());
    };

    auto handle_step = [&](std::int64_t step) {
        if (step % config.logging_interval == 0) {
            print_progress(step);
        }
        if (step % config.sampling_interval == 0) {
            save_snapshot();
        }
    };

    // Simulation driver
    auto simulate_step = [&] {
        std::uniform_int_distribution<std::size_t> index_distr{0, num_spins - 1};

        auto const index = index_distr(random);
        auto const width = config.size;
        auto const beta = 1 / config.temperature;

        auto const top = (index + num_spins - width) % num_spins;
        auto const left = (index + num_spins - 1) % num_spins;
        auto const right = (index + 1) % num_spins;
        auto const bottom = (index + width) % num_spins;

        auto const spin = spins[index];
        auto const neighbor =
            spins[top] + spins[left] + spins[right] + spins[bottom];
        auto const field =
            config.external_field + config.interaction * neighbor;

        auto const glauber =
            std::exp(-beta * field * spin) / (2 * std::cosh(beta * field));
        std::uniform_real_distribution<double> uniform{0, 1};
        if (uniform(random) < glauber) {
            spins[index] = -spin;
        }
    };

    handle_step(0);

    for (std::int64_t step = 1; step <= config.sampling_steps; step++) {
        simulate_step();
        handle_step(step);
    }
}
