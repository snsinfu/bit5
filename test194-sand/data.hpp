#pragma once

#include <cstdint>
#include <string>

#include <md.hpp>


struct simulation_config
{
    md::index     struct_particles;
    md::index     carrier_particles;
    md::scalar    repulsion_energy;
    md::scalar    repulsion_diameter;
    md::scalar    struct_gravity;
    md::scalar    box_energy;
    md::scalar    box_size;
    md::scalar    exhaust_diameter;
    md::scalar    struct_mobility;
    md::scalar    carrier_mobility;
    md::scalar    temperature;
    md::scalar    timestep;
    md::scalar    relaxation_spacestep;
    md::step      relaxation_steps;
    md::step      simulation_steps;
    md::step      sampling_interval;
    md::step      logging_interval;
    std::uint64_t random_seed;
    std::string   output_filename;
};


enum class particle_type
{
    struct_particle,
    carrier_particle,
};


struct simulation_box
{
    md::scalar x_min;
    md::scalar x_max;
    md::scalar y_min;
    md::scalar y_max;
};
