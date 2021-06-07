#include <cmath>
#include <cstddef>
#include <iostream>
#include <vector>


struct simulation_config
{
    std::size_t gene_count       = 1;
    double      activation_speed = 0;
    double      repression_rate  = 0;
    double      bounding_rate    = 0;
    double      expression_speed = 0;
    double      decay_rate       = 0;
    double      initial_u        = 0;
    double      timestep         = 0;
    long        simulation_steps = 0;
    long        output_interval  = 1;
};


class simulation
{
public:
    explicit simulation(simulation_config const& config);
    void     run();

private:
    void run_initialization();
    void run_simulation();
    void update_velocities();
    void update_states();
    void output_header();
    void output_state(long step);

private:
    simulation_config   config;
    std::vector<double> us;
    std::vector<double> xs;
    std::vector<double> u_velocities;
    std::vector<double> x_velocities;
};


static double sigmoid(double x);


int main()
{
    simulation_config const config = {
        .gene_count       = 3,
        .activation_speed = 0.1,
        .repression_rate  = 0.1,
        .bounding_rate    = 0.005,
        .expression_speed = 2.0,
        .decay_rate       = 0.5,
        .initial_u        = 1,
        .timestep         = 1e-3,
        .simulation_steps = 10'000'000,
        .output_interval  = 500,
    };

    simulation(config).run();
}


simulation::simulation(simulation_config const& config)
    : config(config)
    , us(config.gene_count)
    , xs(config.gene_count)
    , u_velocities(config.gene_count)
    , x_velocities(config.gene_count)
{
}


void simulation::run()
{
    run_initialization();
    run_simulation();
}


void simulation::run_initialization()
{
    us[0] = config.initial_u;
}


void simulation::run_simulation()
{
    output_header();
    output_state(0);

    for (long step = 1; step <= config.simulation_steps; step++) {
        update_velocities();
        update_states();

        if (step % config.output_interval == 0) {
            output_state(step);
        }
    }
}


void simulation::update_velocities()
{
    auto const n = config.gene_count;
    auto const a = config.activation_speed;
    auto const r = config.repression_rate;
    auto const h = config.bounding_rate;
    auto const g = config.expression_speed;
    auto const k = config.decay_rate;

    for (std::size_t i = 0; i < n; i++) {
        auto const u = us[i];
        auto const x = xs[i];
        auto const x_pre = xs[(i + n - 1) % n];
        auto const phi = sigmoid(u);

        u_velocities[i] = a - r * x_pre - h * u;
        x_velocities[i] = g * phi - k * x;
    }
}


void simulation::update_states()
{
    auto const dt = config.timestep;

    for (std::size_t i = 0; i < config.gene_count; i++) {
        us[i] += u_velocities[i] * dt;
        xs[i] += x_velocities[i] * dt;
    }
}


void simulation::output_header()
{
    std::cout << "step";

    for (std::size_t i = 0; i < config.gene_count; i++) {
        std::cout << "\tu" << i + 1;
    }

    for (std::size_t i = 0; i < config.gene_count; i++) {
        std::cout << "\tx" << i + 1;
    }

    std::cout << '\n';
}


void simulation::output_state(long step)
{
    std::cout << step;

    for (auto const u : us) {
        std::cout << '\t' << std::nearbyint(u * 100) / 100;
    }

    for (auto const x : xs) {
        std::cout << '\t' << std::nearbyint(x * 100) / 100;
    }

    std::cout << '\n';
}


double sigmoid(double x)
{
    return 1 / (1 + std::exp(-x));
}
