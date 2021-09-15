#include <iostream>
#include <random>
#include <string>

#include <h5.hpp>
#include <md.hpp>


int main()
{
    md::index   const particles = 1000;
    md::scalar  const diameter  = 0.1;
    md::scalar  const repulsion = 10.0;
    md::scalar  const box_size  = 1.0;
    md::scalar  const timestep  = 1e-4;
    md::step    const steps     = 1000000;
    md::step    const interval  = 1000;
    std::string const filename  = "_output.h5";

    md::system system;
    md::random_engine random;

    // Setup

    std::uniform_real_distribution<md::scalar> coord{0, box_size};
    for (md::index i = 0; i < particles; i++) {
        system.add_particle({
            .position = {coord(random), coord(random), coord(random)},
        });
    }

    system.add_forcefield(
        md::make_neighbor_pairwise_forcefield<md::periodic_box>(
            md::softcore_potential<6, 4>{
                .energy   = repulsion,
                .diameter = diameter,
            }
        )
        .set_unit_cell({
            .x_period = box_size,
            .y_period = box_size,
            .z_period = box_size,
        })
        .set_neighbor_distance(diameter)
    );

    // Sampling

    h5::file output{filename, "w"};

    auto positions_dataset = output.dataset<h5::f32, 3>("positions_history");
    auto positions_stream = positions_dataset.stream_writer(
        h5::shape<2>{system.particle_count(), 3}, {.compression = 1}
    );

    auto callback = [&](md::step step) {
        if (step % interval == 0) {
            auto const positions = system.view_positions();
            positions_stream.write(&positions.data()->x);
        }
    };

    callback(0);

    md::simulate_brownian_dynamics(system, {
        .steps    = steps,
        .timestep = timestep,
        .seed     = random(),
        .callback = callback,
    });
}
