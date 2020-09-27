#include <cmath>
#include <iostream>
#include <random>

#include <h5.hpp>
#include <md.hpp>

#include "system_builder.hpp"


int main()
{
    simulation_config const config = {
        .a_particles            = 1000,
        .b_particles            = 1000,
        .a_radius               = 0.07,
        .b_radius               = 0.07,
        .a_repulsion            = 1,
        .b_repulsion            = 1,
        .box_size               = 1,
        .simulation_timestep    = 1e-5,
        .simulation_steps       = 500000,
        .logging_interval       = 1000,
        .sampling_interval      = 100,
        .random_seed            = 0,
        .output_filename        = "output.h5",
    };

    md::system system = system_builder(config).build();
    std::mt19937_64 random(config.random_seed);
    h5::file output(config.output_filename, "w");

    // Initialization

    for (md::point& pos : system.view_positions()) {
        std::uniform_real_distribution<md::scalar> coord{0, config.box_size};
        pos = { coord(random), coord(random), coord(random) };
    }

    // Production sampling

    auto callback = [&](md::step step) {
        if (step % config.logging_interval == 0) {
            auto const mean_energy = system.compute_potential_energy() / system.particle_count();
            std::clog
                << step
                << '\t'
                << mean_energy
                << '\n';
        }

        if (step % config.sampling_interval == 0) {
            auto const step_name = std::to_string(step);
            auto const sample_key = "samples/" + step_name;
            auto const positions = system.view_positions();
            auto const velocities = system.view_velocities();

            std::vector<md::vector> forces(system.particle_count());
            system.compute_force(forces);

            std::vector<std::string> steps;
            auto steps_dataset = output.dataset<h5::str, 1>("samples/steps");
            if (steps_dataset) {
                steps_dataset.read_fit(steps);
            }
            steps.push_back(step_name);

            output.dataset<h5::f32, 2>(sample_key + "/positions").write(
                &positions.data()->x, {positions.size(), 3}, {.compression = 1}
            );

            output.dataset<h5::f32, 2>(sample_key + "/velocities").write(
                &velocities.data()->x, {velocities.size(), 3}, {.compression = 1}
            );

            output.dataset<h5::f32, 2>(sample_key + "/forces").write(
                &forces.data()->x, {forces.size(), 3}, {.compression = 1}
            );

            steps_dataset.write(steps);
        }
    };

    callback(0);

    md::simulate_brownian_dynamics(system, {
        .temperature = 0,
        .timestep    = config.simulation_timestep,
        .steps       = config.simulation_steps,
        .callback    = callback,
    });
}
