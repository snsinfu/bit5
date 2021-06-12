#include <algorithm>
#include <cmath>
#include <cstddef>
#include <cstdint>
#include <iostream>
#include <random>


struct simulation_config
{
    std::size_t   genes;
    double        generation_rate;
    double        decay_rate;
    double        adsorption;
    double        tau;
    long          steps;
    std::uint64_t random_seed;
};


class simulation
{
public:
    explicit simulation(simulation_config const& config);
    void     run();

private:
    void update_deltas();
    void update_amounts();
    void print_state(long step) const;

private:
    simulation_config config;
    std::mt19937_64   random;
    std::vector<int>  amounts;
    std::vector<int>  deltas;
};


template<typename RNG>
static int poisson(double lambda, RNG& random);


int main()
{
    simulation_config const config = {
        .genes            = 3,
        .generation_rate  = 1.0,    // 1 per hour
        .decay_rate       = 0.5,    // 0.5 per hour
        .adsorption       = 20.0,   // strong
        .tau              = 0.1,    // 0.1 hours
        .steps            = 2000,   // 200 hours (1+ week)
        .random_seed      = 0,
    };
    simulation(config).run();
}


simulation::simulation(simulation_config const& config)
    : config(config)
    , amounts(config.genes)
    , deltas(config.genes)
{
    std::seed_seq seed_seq = {config.random_seed};
    random.seed(seed_seq);
}


void simulation::run()
{
    print_state(0);

    for (long step = 1; step < config.steps; step++) {
        update_deltas();
        update_amounts();
        print_state(step);
    }
}


void simulation::update_deltas()
{
    // Compute tau-leaing delta. The corresponding ODE (which we DO NOT compute
    // but is convenient to illustrate the system) is:
    //
    //   d/dt x(i) = g phi(i) - k x(i)
    //   phi(i) = 1 / (1 + a x(i+1))
    //
    // x(i): expression amount of i-th gene
    // phi(i): regulation state of i-th gene (0: off, 1: on)
    // (The indexing is assumed to be cyclic.)
    //
    // g: basic expression rate
    // k: decay rate
    // a: repressor adsorption boltzmann factor exp(-β (ε - μ))
    //
    // The single-particle chemical potential μ of a protein of typical mass
    // 50 kDa is estimated to be βμ ~ -50 in a nucleus of volume 200 μm^3
    // assuming an ideal gas. The binding free energy of a DNA binding protein
    // is around ~30 kcal/mol, which is βε ~ -50, hence a ~ 1. If the binding
    // free energy is a bit stronger 33kcal/mol, we have βε = -55, which gives
    // far larger a ~ 148. So, it is biophysically reasonable to tune the
    // parameter a in 1 to 1000.

    auto const n = config.genes;
    auto const g = config.generation_rate;
    auto const k = config.decay_rate;
    auto const a = config.adsorption;
    auto const tau = config.tau;

    for (std::size_t i = 0; i < n; i++) {
        auto const x_i = amounts[i];
        auto const x_r = amounts[(i + 1) % n];
        auto const phi = 1 / (1 + a * x_r);

        auto const g_mean = tau * g * phi;
        auto const k_mean = tau * k * x_i;

        deltas[i] = poisson(g_mean, random) - poisson(k_mean, random);
    }
}


void simulation::update_amounts()
{
    for (std::size_t i = 0; i < config.genes; i++) {
        amounts[i] = std::max(amounts[i] + deltas[i], 0);
    }
}


void simulation::print_state(long step) const
{
    auto const simulated_time = double(step) * config.tau;

    std::cout << simulated_time;

    for (auto const amount : amounts) {
        std::cout << '\t' << amount;
    }
    std::cout << '\n';
}


template<typename RNG>
int poisson(double lambda, RNG& random)
{
    std::poisson_distribution<int> distr(lambda);
    return distr(random);
}
