#pragma once

#include <md.hpp>

#include "particle_data.hpp"
#include "simulation_config.hpp"


class system_builder
{
public:
    system_builder(simulation_config const& config);

    md::system build();

private:
    void setup_particles();
    void setup_forcefield();

private:
    simulation_config _config;
    md::system        _system;
};
