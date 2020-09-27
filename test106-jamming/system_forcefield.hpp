#pragma once

#include <algorithm>

#include <md.hpp>

#include "particle_data.hpp"
#include "simulation_config.hpp"


class system_forcefield
    : public md::basic_neighbor_pairwise_forcefield<system_forcefield, md::periodic_box>
{
public:
    explicit system_forcefield(simulation_config const& config);
    auto neighbor_pairwise_potential(md::system const& system, md::index i, md::index j) const;

private:
    md::softcore_potential<2, 3> _a_potential;
    md::softcore_potential<2, 8> _b_potential;
};

inline
system_forcefield::system_forcefield(simulation_config const& config)
{
    _a_potential = {
        .energy   = config.a_repulsion,
        .diameter = config.a_radius * 2,
    };

    _b_potential = {
        .energy   = config.b_repulsion,
        .diameter = config.b_radius * 2,
    };

    set_unit_cell({
        .x_period = config.box_size,
        .y_period = config.box_size,
        .z_period = config.box_size,
    });

    auto const max_diameter = std::max(config.a_radius, config.b_radius) * 2;
    set_neighbor_distance(max_diameter);
}

inline auto
system_forcefield::neighbor_pairwise_potential(md::system const& system, md::index i, md::index j) const
{
    auto const data = system.view(particle_data_attribute);
    auto const a_factor = (data[i].a_factor + data[j].a_factor) / 2;
    auto const b_factor = (data[i].b_factor + data[j].b_factor) / 2;
    return a_factor * _a_potential + b_factor * _b_potential;
}
