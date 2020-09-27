#pragma once

#include <cstdint>
#include <string>

#include <md.hpp>


struct simulation_config
{
    md::index     a_particles;
    md::index     b_particles;
    md::scalar    a_radius;
    md::scalar    b_radius;
    md::scalar    a_repulsion;
    md::scalar    b_repulsion;
    md::scalar    box_size;
    md::scalar    simulation_timestep;
    md::step      simulation_steps;
    md::step      logging_interval;
    md::step      sampling_interval;
    std::uint64_t random_seed;
    std::string   output_filename;
};
