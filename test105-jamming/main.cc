#include <cmath>
#include <iostream>
#include <random>
#include <string>
#include <vector>

#include <h5.hpp>
#include <md.hpp>

#include "format.hpp"
#include "power_potential.hpp"


int main()
{
    h5::file output("output.h5", "w");
    md::system system;
    std::mt19937_64 random;

    constexpr md::scalar temperature = 0.1;
    constexpr md::scalar box_size = 1.0;
    constexpr md::scalar particle_repulsion = 1.0;
    constexpr md::scalar particle_diameter = 0.1;
    constexpr md::index particle_count = 1200;

    auto const box_volume = std::pow(box_size, 3);
    auto const particle_volume = 6.2832 * std::pow(particle_diameter / 2, 3);
    auto const volume_fraction = md::scalar(particle_count) * particle_volume / box_volume;
    std::clog << "volume fraction: " << volume_fraction << '\n';

    system.add_forcefield(
        md::make_neighbor_pairwise_forcefield<md::periodic_box>(
            power_potential<2> {
                .energy          = particle_repulsion,
                .cutoff_distance = particle_diameter,
            }
        )
        .set_unit_cell({
            .x_period = box_size,
            .y_period = box_size,
            .z_period = box_size,
        })
        .set_neighbor_distance(particle_diameter)
    );

    for (md::index i = 0; i < particle_count; i++) {
        std::uniform_real_distribution<md::scalar> coord{0, box_size};
        system.add_particle({
            .position = {coord(random), coord(random), coord(random)},
        });
    }

    auto show_progress = [&](md::step step) {
        auto const mean_energy = system.compute_energy() / system.particle_count();
        std::clog << step << '\t' << mean_energy << '\n';
    };

    auto save_sample = [&](md::step step) {
        auto const positions = system.view_positions();
        std::vector<md::vector> forces(system.particle_count());
        system.compute_force(forces);

        output.dataset<h5::f32, 2>(format("samples/", step, "/positions")).write(
            &positions.data()->x,
            {positions.size(), 3},
            {.compression = 1, .scaleoffset = 6}
        );

        output.dataset<h5::f32, 2>(format("samples/", step, "/forces")).write(
            &forces.data()->x,
            {forces.size(), 3},
            {.compression = 1, .scaleoffset = 6}
        );

        std::vector<std::string> steps;
        auto steps_dataset = output.dataset<h5::str, 1>("samples/steps");
        if (steps_dataset) {
            steps_dataset.read_fit(steps);
        }
        steps.push_back(format(step));
        steps_dataset.write(steps);
    };

    auto callback = [&](md::step step) {
        if (step % 10000 == 0) {
            show_progress(step);
        }

        if (step % 100 == 0) {
            save_sample(step);
        }
    };

    md::simulate_brownian_dynamics(system, {
        .temperature = 0,
        .timestep    = 1e-5,
        .steps       = 100000,
    });

    callback(0);

    md::simulate_brownian_dynamics(system, {
        .temperature = temperature,
        .timestep    = 1e-5,
        .steps       = 100000,
        .callback    = callback,
    });
}
