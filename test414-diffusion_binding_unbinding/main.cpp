#include <cmath>
#include <cstddef>
#include <iostream>
#include <optional>
#include <ostream>
#include <random>
#include <vector>


// Reaction-diffusion system. The system consists of "carriers" and "particles"
// on a one-dimensional periodic line. Particles freely diffuse, occassionally
// binding to and unbinding from carriers. Carriers drift towards one direction.
// So, the particles are transported by the carriers. The system should exhibit
// a pattern of dense and sparse regions of unbound particles.


class simulation
{
public:
    struct config_type
    {
        double      width                = 0;
        std::size_t carriers             = 0;
        std::size_t particles            = 0;
        double      carrier_velocity     = 0;
        double      carrier_noise        = 0;
        double      particle_diffusivity = 0;
        double      binding_rate         = 0;
        double      unbinding_rate       = 0;
        double      binding_radius       = 0;
    };

    using random_type = std::mt19937_64;

    explicit simulation(config_type const& config);

    void initialize(random_type& random);
    void step(double dt, random_type& random);
    void print(std::ostream& out);

private:
    void   step_motion(double dt, random_type& random);
    void   step_binding(double dt, random_type& random);
    void   step_unbinding(double dt, random_type& random);
    double distance(double xa, double xb) const;

    struct carrier_data
    {
        double position = 0;
        bool   bound    = false;
    };

    struct particle_data
    {
        double                     position = 0;
        std::optional<std::size_t> carrier;
    };

private:
    config_type                _config;
    std::vector<carrier_data>  _carriers;
    std::vector<particle_data> _particles;
};


int
main()
{
    std::mt19937_64 random;

    simulation sim({
        .width                = 1.0,
        .carriers             = 10,
        .particles            = 10,
        .carrier_velocity     = 1.0,
        .carrier_noise        = 0.01,
        .particle_diffusivity = 0.01,
        .binding_rate         = 20.0,
        .unbinding_rate       = 5.0,
        .binding_radius       = 0.05,
    });
    double const dt = 0.01;
    long const steps = 1000;

    sim.initialize(random);
    sim.print(std::cout);

    for (long step = 1; step <= steps; step++) {
        sim.step(dt, random);
        sim.print(std::cout);
    }
}


namespace
{
    template<typename T>
    std::size_t
    index(std::vector<T> const& vec, T const& elem)
    {
        return static_cast<std::size_t>(&elem - vec.data());
    }
}


simulation::simulation(config_type const& config)
    : _config{config}
    , _carriers(_config.carriers)
    , _particles(_config.particles)
{
}


void
simulation::initialize(random_type& random)
{
    // Carriers are stacked at the origin.
    for (auto& carrier : _carriers) {
        carrier = {
            .position = 0,
            .bound    = false,
        };
    }

    // Particles are uniformly distributed.
    std::uniform_real_distribution<double> coord{0, _config.width};
    for (auto& particle : _particles) {
        particle = {
            .position = coord(random),
            .carrier  = std::nullopt,
        };
    }
}


void
simulation::step(double dt, random_type& random)
{
    step_motion(dt, random);
    step_unbinding(dt, random);
    step_binding(dt, random);
}


void
simulation::step_motion(double dt, random_type& random)
{
    std::normal_distribution<double> normal;

    // Carriers
    for (auto& carrier : _carriers) {
        auto const drift = _config.carrier_velocity * dt;
        auto const noise = std::sqrt(2 * _config.carrier_noise * dt) * normal(random);
        carrier.position += drift + noise;
    }

    // Particles
    for (auto& particle : _particles) {
        if (particle.carrier) {
            particle.position = _carriers[*particle.carrier].position;
        } else {
            particle.position += std::sqrt(2 * _config.particle_diffusivity * dt) * normal(random);
        }
    }
}


void
simulation::step_unbinding(double dt, random_type& random)
{
    for (auto& particle : _particles) {
        if (!particle.carrier) {
            continue;
        }

        std::bernoulli_distribution unbinding{_config.binding_rate * dt};
        if (unbinding(random)) {
            auto& carrier = _carriers[*particle.carrier];
            particle.carrier = std::nullopt;
            carrier.bound = false;
        }
    }
}


void
simulation::step_binding(double dt, random_type& random)
{
    std::vector<std::size_t> candidates;

    for (auto& particle : _particles) {
        if (particle.carrier) {
            continue;
        }

        // Find unbound carriers around a particle.
        candidates.clear();

        for (auto& carrier : _carriers) {
            if (carrier.bound) {
                continue;
            }

            if (distance(particle.position, carrier.position) < _config.binding_radius) {
                candidates.push_back(index(_carriers, carrier));
            }
        }

        // Determine the first binding event within given time interval dt.
        auto const total_rate = _config.binding_rate * double(candidates.size());
        std::exponential_distribution<double> latency{total_rate};

        if (latency(random) < dt) {
            // We are using the same binding rate for all carriers, so all
            // candidates are equally likely.
            std::uniform_int_distribution<std::size_t> choice{0, candidates.size() - 1};
            auto const index = candidates[choice(random)];
            auto& carrier = _carriers[index];

            particle.carrier = index;
            carrier.bound = true;
        }
    }
}


double
simulation::distance(double xa, double xb) const
{
    auto const delta = xa - xb;
    auto const period = _config.width;
    return std::fabs(delta - std::round(delta / period) * period);
}


void
simulation::print(std::ostream& out)
{
    for (auto const& particle : _particles) {
        if (index(_particles, particle)) {
            out << ' ';
        }

        if (particle.carrier) {
            out << "nan";
        } else {
            out << particle.position;
        }
    }
    out << '\n';
}
