#include "system_builder.hpp"

#include <algorithm>
#include <utility>

#include "system_forcefield.hpp"


system_builder::system_builder(simulation_config const& config)
    : _config{config}
{
}

md::system
system_builder::build()
{
    setup_particles();
    setup_forcefield();
    return std::move(_system);
}

void
system_builder::setup_particles()
{
    _system.require(particle_data_attribute);

    for (md::index i = 0; i < _config.a_particles; i++) {
        _system.add_particle().view(particle_data_attribute) = {
            .a_factor = 1,
            .b_factor = 0,
        };
    }

    for (md::index i = 0; i < _config.b_particles; i++) {
        _system.add_particle().view(particle_data_attribute) = {
            .a_factor = 0,
            .b_factor = 1,
        };
    }
}

void
system_builder::setup_forcefield()
{
    _system.add_forcefield(system_forcefield(_config));
}
