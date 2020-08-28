#include <cassert>
#include <cmath>
#include <cstdint>
#include <functional>
#include <iostream>
#include <random>


using std::uint64_t;

struct simulation_sample {
    double position;
    double velocity;
    double kinetic_energy;
    double potential_energy;
};

using sampler_type = std::function<void(long, simulation_sample const&)>;

struct simulation_config {
    double       temperature;
    double       mass;
    double       friction;
    double       spring_constant;
    double       timestep;
    long         sampling_steps;
    long         sampling_interval;
    uint64_t     seed;
    sampler_type sampler;
};

void run_simulation(simulation_config const& config);


int main()
{
    double mean_kinetic_energy = 0;
    double mean_potential_energy = 0;
    double variance = 0;
    double samples = 0;

    auto sampler = [&](long step, simulation_sample const& sample) {
        (void) step;
        mean_kinetic_energy += sample.kinetic_energy;
        mean_potential_energy += sample.potential_energy;
        variance += sample.position * sample.position;
        samples++;
    };

    // - Try modifying temperature, mass and spring_constant
    // - Canonical sampling works unless friction is too small
    simulation_config const config = {
        .temperature       = 1,
        .mass              = 1,
        .friction          = 1,
        .spring_constant   = 10,
        .timestep          = 1e-3,
        .sampling_steps    = 10000000,
        .sampling_interval = 1000,
        .seed              = 0,
        .sampler           = sampler,
    };

    auto const expected_variance = config.temperature / config.spring_constant;
    auto const expected_kinetic_energy = config.temperature / 2;
    auto const expected_potential_energy = config.temperature / 2;

    run_simulation(config);

    // Test against canonical statistics

    mean_kinetic_energy /= samples;
    mean_potential_energy /= samples;
    variance /= samples;

    std::cout
        << "Mean kinentic energy:\n"
        << "  actual = " << mean_kinetic_energy
        << '\n'
        << "  expect = " << expected_kinetic_energy
        << '\n';

    std::cout
        << "Mean potential energy:\n"
        << "  actual = " << mean_potential_energy
        << '\n'
        << "  expect = " << expected_potential_energy
        << '\n';

    std::cout
        << "Position variance:\n"
        << "  actual = " << variance
        << '\n'
        << "  expect = " << expected_variance 
        << '\n';

    assert(std::fabs(mean_kinetic_energy / expected_kinetic_energy - 1) < 0.1);
    assert(std::fabs(mean_potential_energy / expected_potential_energy - 1) < 0.1);
    assert(std::fabs(variance / expected_variance - 1) < 0.1);
}


void run_simulation(simulation_config const& config)
{
    std::mt19937_64 random;
    {
        std::seed_seq seed{config.seed};
        random.seed(seed);
    }

    double x = 0;
    double v = 0;
    double F = 0;

    auto compute_energy = [&] {
        return config.spring_constant / 2 * x * x;
    };

    auto compute_force = [&] {
        return -config.spring_constant * x;
    };

    std::normal_distribution<double> normal;

    for (long step = 1; step <= config.sampling_steps; step++) {
        auto const m = config.mass;
        auto const kT = config.temperature;
        auto const dt = config.timestep;
        auto const damping = std::exp(-dt * config.friction);
        auto const fluctuation = -std::expm1(-2 * dt * config.friction);
        auto const sigma = std::sqrt(kT / m * fluctuation);

        // BAOAB discretization scheme for Langevin equations of motion
        // https://doi.org/10.1063/1.4802990

        v += dt / 2 / m * F;
        x += dt / 2 * v;
        v *= damping;
        v += sigma * normal(random);
        x += dt / 2 * v;

        F = compute_force();

        v += dt / 2 / m * F;

        if (step % config.sampling_interval == 0) {
            simulation_sample const sample = {
                .position         = x,
                .velocity         = v,
                .potential_energy = config.mass / 2 * v * v,
                .kinetic_energy   = compute_energy(),
            };
            config.sampler(step, sample);
        }
    }
}
