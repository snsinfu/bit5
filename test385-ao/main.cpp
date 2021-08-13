#include <algorithm>
#include <cstdint>
#include <iostream>
#include <random>
#include <string>

#include <h5.hpp>
#include <md.hpp>


struct simulation_config
{
    md::scalar    box_size;
    md::index     small_particles;
    md::index     large_particles;
    md::scalar    small_radius;
    md::scalar    large_radius;
    md::scalar    repulsion_energy;
    md::scalar    timestep;
    md::step      steps;
    md::step      sampling_interval;
    md::step      logging_interval;
    std::uint64_t seed;
    std::string   output_filename;
};


struct particle_data
{
    md::scalar radius  = 0;
    bool       is_soft = true;
};

md::attribute_key<particle_data> particle_data_attribute;


int main()
{
    simulation_config const config = {
        .box_size          = 1.0,
        .small_particles   = 100,
        .large_particles   = 10,
        .small_radius      = 0.1,
        .large_radius      = 0.2,
        .repulsion_energy  = 10,
        .timestep          = 1e-4,
        .steps             = 1000000,
        .sampling_interval = 1000,
        .logging_interval  = 1000,
        .seed              = 0,
        .output_filename   = "_output.h5",
    };

    md::system system;
    std::mt19937_64 random{config.seed};
    h5::file store{config.output_filename, "w"};

    // Define particles

    system.require(particle_data_attribute);

    for (md::index i = 0; i < config.small_particles; i++) {
        system.add_particle().view(particle_data_attribute) = {
            .radius  = config.small_radius,
            .is_soft = true,
        };
    }

    for (md::index i = 0; i < config.large_particles; i++) {
        system.add_particle().view(particle_data_attribute) = {
            .radius  = config.large_radius,
            .is_soft = false,
        };
    }

    // Define forcefield

    auto const max_diameter = 2 * std::max(config.small_radius, config.large_radius);

    system.add_forcefield(
        md::make_neighbor_pairwise_forcefield<md::periodic_box>(
            [=](md::index i, md::index j) {
                auto const data = system.view(particle_data_attribute);
                md::softcore_potential<4, 3> potential = {
                    .energy   = config.repulsion_energy,
                    .diameter = data[i].radius + data[j].radius,
                };
                if (data[i].is_soft && data[j].is_soft) {
                    potential.energy = 0;
                }
                return potential;
            }
        )
        .set_unit_cell({
            .x_period = config.box_size,
            .y_period = config.box_size,
            .z_period = config.box_size,
        })
        .set_neighbor_distance(max_diameter)
    );

    // Initialize
    {
        std::uniform_real_distribution<md::scalar> coord{0, config.box_size};
        for (auto& pos : system.view_positions()) {
            pos = {coord(random), coord(random), coord(random)};
        }
    }

    // Simulation

    auto positions_dataset = store.dataset<h5::f32, 3>("positions_history");
    auto positions_stream = positions_dataset.stream_writer(
        {system.particle_count(), 3}, {.compression = 1}
    );

    auto const callback = [&](md::step step) {
        if (step % config.logging_interval == 0) {
            std::clog
                << step
                << "\tE: "
                << system.compute_energy() / md::scalar(system.particle_count())
                << '\n';
        }

        if (step % config.sampling_interval == 0) {
            positions_stream.write(&system.view_positions().data()->x);
        }
    };

    callback(0);

    md::simulate_brownian_dynamics(system, {
        .temperature = 1,
        .timestep    = config.timestep,
        .steps       = config.steps,
        .seed        = random(),
        .callback    = callback,
    });
}
