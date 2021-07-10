#include <algorithm>
#include <cmath>
#include <cstddef>
#include <cstdint>
#include <iostream>
#include <numbers>
#include <random>

#include <md.hpp>


struct simulation_config
{
    std::size_t   chain_length     = 0;
    md::scalar    bead_repulsion   = 0;
    md::scalar    bead_diameter    = 0;
    md::scalar    bond_spring      = 0;
    md::scalar    charge_spring    = 0;
    int           charge_mobility  = 0;
    md::scalar    temperature      = 0;
    md::scalar    timestep         = 0;
    md::step      steps            = 0;
    md::step      logging_interval = 0;
    std::uint64_t random_seed      = 0;
};


struct particle_data
{
    md::scalar charge = 0;
};


md::attribute_key<particle_data> particle_data_attribute;


class simulation
{
public:
    simulation(simulation_config const& config);

    void run();

private:
    void setup_particles();
    void setup_forcefield();
    void setup_forcefield_bonds();
    void setup_forcefield_charges();
    void run_initialization();
    void run_initialization_structure();
    void run_initialization_charges();
    void run_sampling();
    void move_charge();
    void show_progress(md::step step);

private:
    simulation_config config;
    md::system        system;
    std::mt19937_64   random;
};


static std::mt19937_64 make_random(std::uint64_t seed);


int main()
{
    simulation_config const config = {
        .chain_length     = 100,
        .bead_repulsion   = 5,
        .bead_diameter    = 0.02,
        .bond_spring      = 100,
        .charge_spring    = 1000,
        .charge_mobility  = 10,
        .temperature      = 0.2,
        .timestep         = 1e-4,
        .steps            = 10000,
        .logging_interval = 5,
        .random_seed      = 0,
    };
    simulation(config).run();
}


simulation::simulation(simulation_config const& config)
    : config(config)
    , random(make_random(config.random_seed))
{
}


void simulation::run()
{
    setup_particles();
    setup_forcefield();
    run_initialization();
    run_sampling();
}


void simulation::setup_particles()
{
    system.require(particle_data_attribute);

    for (std::size_t i = 0; i < config.chain_length; i++) {
        system.add_particle();
    }
}


void simulation::setup_forcefield()
{
    setup_forcefield_bonds();
    setup_forcefield_charges();
}


void simulation::setup_forcefield_bonds()
{
    auto bonds = system.add_forcefield(
        md::make_bonded_pairwise_forcefield(
            md::harmonic_potential {
                .spring_constant = config.bond_spring
            }
        )
    );

    for (std::size_t i = 0; i + 1 < config.chain_length; i++) {
        bonds->add_bonded_pair(i, i + 1);
    }
}


void simulation::setup_forcefield_charges()
{
    system.add_forcefield(
        md::make_bruteforce_pairwise_forcefield(
            [=](md::index i, md::index j) {
                auto const data = system.view(particle_data_attribute);
                auto const qi = data[i].charge;
                auto const qj = data[j].charge;

                auto const repulsive_potential = md::softcore_potential<2, 3> {
                    .energy   = config.bead_repulsion,
                    .diameter = config.bead_diameter,
                };

                auto const charge_potential = md::harmonic_potential {
                    .spring_constant = qi * qj * config.charge_spring
                };
                return repulsive_potential + charge_potential;
            }
        )
    );
}


void simulation::run_initialization()
{
    run_initialization_structure();
    run_initialization_charges();
}


void simulation::run_initialization_structure()
{
    auto positions = system.view_positions();

    // Initialize as a ring polymer.
    auto const step = config.bead_diameter;
    auto const radius = (positions.size() * step) / (2 * std::numbers::pi);

    for (std::size_t i = 0; i < positions.size(); i++) {
        auto const angle = 2 * std::numbers::pi * i / positions.size();

        positions[i] = {
            radius * std::cos(angle),
            radius * std::sin(angle),
            0,
        };
    }
}


void simulation::run_initialization_charges()
{
    auto data = system.view(particle_data_attribute);

    data.front().charge = 1;
    data.back().charge = 1;
}


void simulation::run_sampling()
{
    auto callback = [&](md::step step) {
        move_charge();

        if (step % config.logging_interval == 0) {
            show_progress(step);
        }
    };

    callback(0);

    md::simulate_brownian_dynamics(system, {
        .temperature = config.temperature,
        .timestep    = config.timestep,
        .steps       = config.steps,
        .callback    = callback,
    });
}


void simulation::move_charge()
{
    auto data = system.view(particle_data_attribute);

    std::uniform_int_distribution<std::size_t> index(1, data.size() - 1);
    for (int i = 0; i < config.charge_mobility; i++) {
        auto const pos = index(random);
        std::swap(data[pos].charge, data[pos - 1].charge);
    }
}


void simulation::show_progress(md::step step)
{
    auto const mean_energy = system.compute_energy() / system.particle_count();

    std::clog
        << step
        << "\tE: "
        << mean_energy
        << '\n';

    auto const positions = system.view_positions();
    auto const data = system.view(particle_data_attribute);

    for (std::size_t i = 0; i < system.particle_count(); i++) {
        std::cout << ' ' << positions[i] << ' ' << data[i].charge;
    }
    std::cout << '\n';
}


std::mt19937_64 make_random(std::uint64_t seed)
{
    std::seed_seq seed_seq{seed};
    return std::mt19937_64(seed_seq);
}
