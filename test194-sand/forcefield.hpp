#pragma once

#include <vector>

#include <md.hpp>

#include "data.hpp"


class box_forcefield : public md::forcefield
{
public:
    box_forcefield(
        simulation_config const& config,
        simulation_box const& box,
        std::vector<particle_type> const& particle_types
    )
        : _particle_types{particle_types}
        , _box{box}
        , _exhaust_diameter{config.exhaust_diameter}
        , _potential{make_potential(config)}
    {
    }


    md::scalar
    compute_energy(md::system const& system) override
    {
        md::scalar sum = 0;

        loop_interaction(system, [&](md::index, md::vector const& r) {
            sum += _potential.evaluate_energy(r);
        });

        return sum;
    }


    void
    compute_force(md::system const& system, md::array_view<md::vector> forces) override
    {
        loop_interaction(system, [&](md::index i, md::vector const& r) {
            forces[i] += _potential.evaluate_force(r);
        });
    }


private:
    template<typename Callback>
    void
    loop_interaction(md::system const& system, Callback const& callback) const
    {
        auto const positions = system.view_positions();
        auto const box = _box;
        auto const depth = _potential.diameter;

        auto const exhaust_center = (box.y_min + box.y_max) / 2;
        auto const exhaust_min = exhaust_center - _exhaust_diameter / 2;
        auto const exhaust_max = exhaust_center + _exhaust_diameter / 2;

        for (md::index i = 0; i < positions.size(); i++) {
            auto const pos = positions[i];

            if (pos.x > box.x_min - depth && pos.x < box.x_min + depth) {
                if (_particle_types[i] == particle_type::carrier_particle &&
                        pos.y > exhaust_min &&
                        pos.y < exhaust_max) {
                    // Exhaust region.
                } else {
                    md::vector const r = { pos.x - box.x_min, 0.0 };
                    callback(i, r);
                }
            }

            if (pos.x < box.x_max + depth && pos.x > box.x_max - depth) {
                md::vector const r = { pos.x - box.x_max, 0.0 };
                callback(i, r);
            }

            if (pos.y > box.y_min - depth && pos.y < box.y_min + depth) {
                md::vector const r = { 0.0, pos.y - box.y_min };
                callback(i, r);
            }

            if (pos.y < box.y_max + depth && pos.y > box.y_max - depth) {
                md::vector const r = { 0.0, pos.y - box.y_max };
                callback(i, r);
            }
        }
    }


    static md::softcore_potential<2, 3>
    make_potential(simulation_config const& config)
    {
        return {
            .energy   = config.box_energy,
            .diameter = config.repulsion_diameter / 2,
        };
    }


private:
    std::vector<particle_type> const& _particle_types;
    simulation_box _box;
    md::scalar _exhaust_diameter;
    md::softcore_potential<2, 3> _potential;
};


class struct_forcefield : public md::forcefield
{
public:
    struct_forcefield(
        simulation_config const& config,
        std::vector<particle_type> const& particle_types
    )
        : _particle_types{particle_types}
        , _gravity{config.struct_gravity}
    {
    }


    md::scalar
    compute_energy(md::system const&) override
    {
        return 0;
    }


    void
    compute_force(md::system const&, md::array_view<md::vector> forces) override
    {
        for (md::index i = 0; i < _particle_types.size(); i++) {
            if (_particle_types[i] == particle_type::struct_particle) {
                forces[i] += {-_gravity, 0.0};
            }
        }
    }


private:
    std::vector<particle_type> const& _particle_types;
    md::scalar _gravity;
};
