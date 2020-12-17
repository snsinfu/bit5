// Lotka-Volterra equations:
//
//   dx/dt = Ax - Bxy
//   dy/dt = -Cy + Dxy
//
// Read the equations as these reactions:
//
//        Ax
//   *  ------>  x
//
//        Bxy
//   x  ------>  *
//
//        Cy
//   y  ------>  *
//
//        Dxy
//   *  ------>  y
//

#include <cassert>
#include <cstdint>
#include <iostream>
#include <numeric>
#include <random>
#include <vector>


using std::uint32_t;
using std::uint64_t;


struct system_state
{
    double time;
    std::vector<uint32_t> species;
};

class reaction
{
public:
    virtual ~reaction() = default;
    virtual double rate(system_state const& state) = 0;
    virtual void apply(system_state& state) const = 0;
};

class linear_production : public reaction
{
public:
    linear_production(
        uint32_t target,
        double base_rate,
        std::vector<uint32_t> const& regulators
    )
        : _target{target}
        , _base_rate{base_rate}
        , _regulators{regulators}
    {
    }

    double rate(system_state const& state) override
    {
        double rate = _base_rate;
        for (auto const regulator : _regulators) {
            rate *= double(state.species[regulator]);
        }
        return rate;
    }

    void apply(system_state& state) const override
    {
        state.species[_target] += 1;
    }

private:
    uint32_t _target;
    double _base_rate;
    std::vector<uint32_t> _regulators;
};

class linear_decay : public reaction
{
public:
    linear_decay(
        uint32_t target,
        double base_rate,
        std::vector<uint32_t> const& regulators
    )
        : _target{target}
        , _base_rate{base_rate}
        , _regulators{regulators}
    {
    }

    double rate(system_state const& state) override
    {
        double rate = _base_rate;
        for (auto const regulator : _regulators) {
            rate *= double(state.species[regulator]);
        }
        return rate;
    }

    void apply(system_state& state) const override
    {
        state.species[_target] -= 1;
    }

private:
    uint32_t _target;
    double _base_rate;
    std::vector<uint32_t> _regulators;
};



int main()
{
    // System parameters
    double const alpha = 7.0;
    double const beta = 0.05;
    double const gamma = 15.0;
    double const delta = 0.9;
    uint32_t const initial_prey = 10;
    uint32_t const initial_predator = 10;

    // Simulation parameters
    uint64_t const output_interval = 1;
    uint64_t const simulation_steps = 100000;
    uint64_t const random_seed = 0;

    system_state state = {
        .time    = 0,
        .species = {initial_prey, initial_predator},
    };

    auto output_state = [&](uint64_t step) {
        std::cout
            << step
            << '\t'
            << state.time;
        for (auto const species : state.species) {
            std::cout << '\t' << species;
        }
        std::cout << '\n';
    };

    std::vector<std::unique_ptr<reaction>> reactions;
    reactions.emplace_back(new linear_production{0, alpha, {0}});
    reactions.emplace_back(new linear_decay{0, beta, {0, 1}});
    reactions.emplace_back(new linear_decay{1, gamma, {1}});
    reactions.emplace_back(new linear_production{1, delta, {0, 1}});

    auto const reaction_count = reactions.size();

    std::mt19937_64 random(random_seed);
    std::uniform_real_distribution<double> uniform;

    std::vector<double> rates(reaction_count);
    std::vector<double> sum_rates(reaction_count);

    output_state(0);

    for (uint64_t step = 1; step <= simulation_steps; step++) {
        // Update rates.
        for (std::size_t i = 0; i < reaction_count; i++) {
            rates[i] = reactions[i]->rate(state);
        }
        std::partial_sum(rates.begin(), rates.end(), sum_rates.begin());

        // Determine which reaction to happen.
        auto const total_rate = sum_rates.back();
        auto const stop_rate = uniform(random) * total_rate;

        if (total_rate == 0) {
            std::clog << "Extinction\n";
            break;
        }

        for (std::size_t i = 0; i < reaction_count; i++) {
            if (stop_rate <= sum_rates[i]) {
                reactions[i]->apply(state);
                break;
            }
        }

        // Advance time.
        auto const rr = 1 - uniform(random);
        auto const time_delta = -1 / total_rate * std::log(rr);
        state.time += time_delta;

        if (step % output_interval == 0) {
            output_state(step);
        }
    }
}
