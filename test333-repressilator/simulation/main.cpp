// d/dt phi(i) = phi(i) (1 - phi(i)) (a - r x(i-1))
// d/dt x(i) = g phi(i) - k x(i)

#include <algorithm>
#include <cmath>
#include <cstddef>
#include <iostream>
#include <vector>


struct simulation_config
{
    std::size_t genes;
    double      activation_speed;
    double      repression_rate;
    double      expression_speed;
    double      degradation_rate;
    double      timestep;
    long        steps;
    long        interval;
};

static void run_simulation(simulation_config const& config);


int main()
{
    simulation_config const config = {
        .genes            = 3,
        .activation_speed = 0.10,
        .repression_rate  = 0.11,
        .expression_speed = 2.0,
        .degradation_rate = 0.5,
        .timestep         = 1e-3,
        .steps            = 1000000,
        .interval         = 100,
    };
    run_simulation(config);
}


void run_simulation(simulation_config const& config)
{
    std::vector<double> states(config.genes);
    std::vector<double> proteins(config.genes);
    std::vector<double> next_states(config.genes);
    std::vector<double> next_proteins(config.genes);

    states[0] = 1;

    for (long step = 1; step <= config.steps; step++) {
        for (std::size_t i = 0; i < config.genes; i++) {
            auto const dt = config.timestep;

            // TODO: Refactor this into composable abstractions.

            auto const phi = states[i];
            auto const x = proteins[i];
            auto const x_regulator = proteins[(i + config.genes - 1) % config.genes];

            auto const a = config.activation_speed;
            auto const r = config.repression_rate;
            //auto const phi_pre_velocity = a - r * x_regulator;
            //auto const pre_phi = phi + phi_pre_velocity * dt;
            //auto const phi_velocity = 6 * pre_phi * (1 - pre_phi) * phi_pre_velocity;
            auto const phi_velocity = a - r * x_regulator;

            auto const g = config.expression_speed;
            auto const k = config.degradation_rate;
            auto const x_velocity = g * phi - k * x;

            next_states[i] = phi + phi_velocity * dt;
            next_proteins[i] = x + x_velocity * dt;

            if (next_states[i] < 0) {
                next_states[i] = 0;
            }
            if (next_states[i] > 1) {
                next_states[i] = 1;
            }
        }

        states = next_states;
        proteins = next_proteins;

        if (step % config.interval == 0) {
            std::cout << step;
            for (std::size_t i = 0; i < config.genes; i++) {
                std::cout << '\t' << std::nearbyint(states[i] * 10) / 10;
            }
            std::cout << '\n';
        }
    }
}
