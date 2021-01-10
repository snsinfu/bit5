#include <iostream>
#include <random>
#include <vector>

#include <md.hpp>
#include <h5.hpp>

#include "data.hpp"
#include "forcefield.hpp"


int
main()
{
    simulation_config const config = {
        .struct_particles     = 250,
        .carrier_particles    = 250,
        .repulsion_energy     = 50.0,
        .repulsion_diameter   = 0.1,
        .struct_gravity       = 1.0,
        .box_energy           = 50.0,
        .box_size             = 3.0,
        .exhaust_diameter     = 0.3,
        .struct_mobility      = 0.05,
        .carrier_mobility     = 1.0,
        .temperature          = 1.0,
        .timestep             = 1.0e-5,
        .relaxation_spacestep = 0.001,
        .relaxation_steps     = 10000,
        .simulation_steps     = 2000000,
        .sampling_interval    = 1000,
        .logging_interval     = 1000,
        .random_seed          = 0,
        .output_filename      = "_output.h5",
    };

    simulation_box const box = {
        .x_min = 0,
        .x_max = config.box_size,
        .y_min = -config.box_size / 2,
        .y_max = config.box_size / 2,
    };

    std::mt19937_64 random;
    {
        std::seed_seq seed_seq = {config.random_seed};
        random.seed(seed_seq);
    }

    h5::file output{config.output_filename, "w"};


    // Define particles.
    std::vector<particle_type> particle_types;

    for (md::index i = 0; i < config.struct_particles; i++) {
        particle_types.push_back(particle_type::struct_particle);
    }

    for (md::index i = 0; i < config.carrier_particles; i++) {
        particle_types.push_back(particle_type::carrier_particle);
    }

    {
        h5::enums<int> const enums = {
            {"struct", int(particle_type::struct_particle)},
            {"carrier", int(particle_type::carrier_particle)},
        };

        output.dataset<h5::i32, 1>("particle_types", enums).write(
            reinterpret_cast<int const*>(particle_types.data()),
            {particle_types.size()}
        );
    }


    // Define system.
    md::system system;

    for (auto type : particle_types) {
        auto particle = system.add_particle();

        switch (type) {
        case particle_type::struct_particle:
            particle.mobility = config.struct_mobility;
            break;

        case particle_type::carrier_particle:
            particle.mobility = config.carrier_mobility;
            break;
        }
    }


    // Define forcefield
    system.add_forcefield(
        md::make_neighbor_pairwise_forcefield(
            md::softcore_potential<2, 3> {
                .energy   = config.repulsion_energy,
                .diameter = config.repulsion_diameter,
            }
        )
        .set_neighbor_distance(config.repulsion_diameter)
    );

    system.add_forcefield(box_forcefield {config, box, particle_types});
    system.add_forcefield(struct_forcefield {config, particle_types});


    // Initialize
    {
        auto const positions = system.view_positions();

        for (md::index i = 0; i < particle_types.size(); i++) {
            std::uniform_real_distribution<md::scalar> uniform;
            auto region = box;

            switch (particle_types[i]) {
            case particle_type::struct_particle:
                region.x_max = (region.x_min + region.x_max) / 2;
                break;

            case particle_type::carrier_particle:
                region.x_min = (region.x_min + region.x_max) / 2;
                break;
            }

            positions[i] = {
                region.x_min + (region.x_max - region.x_min) * uniform(random),
                region.y_min + (region.y_max - region.y_min) * uniform(random),
            };
        }
    }

    md::simulate_brownian_dynamics(system, {
        .temperature = 0,
        .timestep    = config.timestep,
        .spacestep   = config.relaxation_spacestep,
        .steps       = config.relaxation_steps,
    });


    // Simulation
    auto positions_dataset = output.dataset<h5::f32, 3>("positions_history");
    auto positions_stream = positions_dataset.stream_writer(
        {system.particle_count(), 2},
        {.scaleoffset = 3, .compression = 1}
    );

    auto callback = [&](md::step step) {
        if (step % config.logging_interval == 0) {
            int exit_count = 0;
            for (auto pos : system.view_positions()) {
                if (pos.x < 0) {
                    exit_count++;
                }
            }

            auto const mean_energy =
                system.compute_energy() / md::scalar(system.particle_count());

            std::clog
                << step
                << '\t'
                << exit_count
                << '\t'
                << mean_energy
                << '\n';
        }

        if (step % config.sampling_interval == 0) {
            auto const positions = system.view_positions();
            positions_stream.write(&positions.data()->x);
        }
    };

    callback(0);

    md::simulate_brownian_dynamics(system, {
        .temperature = config.temperature,
        .timestep    = config.timestep,
        .steps       = config.simulation_steps,
        .callback    = callback,
    });
}
