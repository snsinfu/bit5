#include <cstdint>
#include <exception>
#include <iostream>
#include <optional>
#include <random>
#include <string>

#include <h5.hpp>
#include <md.hpp>


template<>
struct h5::buffer_traits<md::array_view<md::point> const>
{
    using buffer_type = md::array_view<md::point>;
    using value_type = md::scalar;
    static constexpr int rank = 2;
    static constexpr int dimension = 2;

    static h5::shape<rank> shape(buffer_type const& buffer)
    {
        return {buffer.size(), dimension};
    }

    static value_type* data(buffer_type const& buffer)
    {
        return &buffer.data()->x;
    }
};

struct simulation_config
{
    md::index     particle_count;
    md::scalar    repulsion_energy;
    md::scalar    repulsion_diameter;
    md::scalar    box_size;
    md::scalar    temperature;
    md::scalar    timestep;
    md::step      freezing_steps;
    md::step      sampling_steps;
    md::step      sampling_interval;
    md::step      logging_interval;
    std::uint64_t random_seed;
    std::string   output_filename;
};

class simulation
{
public:
    explicit simulation(simulation_config const& config);
    void run();

private:
    void setup_particles();
    void setup_forcefield();
    void setup_output();
    void run_initialization();
    void run_simulation();

    void print_progress(md::step step);
    void save_sample();

private:
    simulation_config const& _config;
    md::system               _system;
    std::mt19937_64          _random;
    h5::file                 _output;

    /** Dataset to store snapshot structures along trajectory. */
    std::optional<h5::dataset<h5::f32, 3>>       _snapshot_dataset;
    std::optional<h5::stream_writer<h5::f32, 2>> _snapshot_stream;

    /** Dataset to store inherent structures along trajectory. */
    std::optional<h5::dataset<h5::f32, 3>>       _inherent_dataset;
    std::optional<h5::stream_writer<h5::f32, 2>> _inherent_stream;
};


int main()
{
    try {
        simulation_config const config = {
            .particle_count     = 1000,
            .repulsion_energy   = 1.0,
            .repulsion_diameter = 0.1,
            .box_size           = 2.8,
            .temperature        = 0.1,
            .timestep           = 1e-5,
            .freezing_steps     = 1'000,
            .sampling_steps     = 1'000'000,
            .sampling_interval  = 1'000,
            .logging_interval   = 10'000,
            .random_seed        = 1,
            .output_filename    = "_output.h5",
        };
        simulation{config}.run();
        return 0;
    } catch (std::exception const& err) {
        std::cerr << err.what() << '\n';
        return 1;
    }
}

simulation::simulation(simulation_config const& config)
    : _config{config}
    , _output{config.output_filename, "w"}
{
    std::seed_seq seed_seq{config.random_seed};
    _random.seed(seed_seq);
}


void simulation::run()
{
    setup_particles();
    setup_forcefield();
    setup_output();
    run_initialization();
    run_simulation();
}

void simulation::setup_particles()
{
    for (md::index i = 0; i < _config.particle_count; i++) {
        _system.add_particle();
    }
}

void simulation::setup_forcefield()
{
    _system.add_forcefield(
        md::make_neighbor_pairwise_forcefield<md::periodic_box>(
            md::softcore_potential<3, 4> {
                .energy   = _config.repulsion_energy,
                .diameter = _config.repulsion_diameter,
            }
        )
        .set_unit_cell({
            .x_period = _config.box_size,
            .y_period = _config.box_size,
        })
        .set_neighbor_distance(_config.repulsion_diameter)
    );
}

void simulation::setup_output()
{
    constexpr std::size_t dimension = 2;

    _snapshot_dataset = _output.dataset<h5::f32, 3>("snapshot_structures");
    _snapshot_stream = _snapshot_dataset->stream_writer(
        {_config.particle_count, dimension}
    );

    _inherent_dataset = _output.dataset<h5::f32, 3>("inherent_structures");
    _inherent_stream = _inherent_dataset->stream_writer(
        {_config.particle_count, dimension}
    );
}

void simulation::run_initialization()
{
    auto positions = _system.view_positions();

    auto const grid_size = std::size_t(std::sqrt(double(_config.particle_count)) + 1);
    auto const grid_interval = _config.box_size / double(grid_size);
    for (std::size_t i = 0; i < positions.size(); i++) {
        auto const grid_x = i % grid_size;
        auto const grid_y = i / grid_size;
        positions[i] = {
            double(grid_x) * grid_interval,
            double(grid_y) * grid_interval,
        };
    }
}

void simulation::run_simulation()
{
    auto handle_step = [&](md::step step) {
        if (step % _config.logging_interval == 0) {
            print_progress(step);
        }
        if (step % _config.sampling_interval == 0) {
            save_sample();
        }
    };

    handle_step(0);

    md::simulate_brownian_dynamics(_system, {
        .temperature = _config.temperature,
        .timestep    = _config.timestep,
        .steps       = _config.sampling_steps,
        .seed        = _random(),
        .callback    = handle_step,
    });
}

void simulation::print_progress(md::step step)
{
    auto const mean_energy = _system.compute_energy() / double(_system.particle_count());

    std::clog
        << step
        << '\t'
        << "E: "
        << mean_energy
        << '\n';
}

void simulation::save_sample()
{
    auto const snapshot = _system.view_positions();
    _snapshot_stream->write(snapshot);

    auto copy_system = _system;
    md::simulate_brownian_dynamics(copy_system, {
        .temperature = 0,
        .timestep    = _config.timestep,
        .steps       = _config.freezing_steps,
    });
    auto const inherent = copy_system.view_positions();
    _inherent_stream->write(inherent);
}
