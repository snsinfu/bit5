#include <algorithm>
#include <cstddef>
#include <cstdint>
#include <cmath>
#include <iostream>
#include <optional>
#include <random>
#include <vector>


struct switch_config
{
    std::optional<std::size_t> gene;
    double                     efficiency = 1;
    double                     rate       = 1;
};


struct gene_config
{
    double        expression_rate = 0;
    double        decay_rate      = 0;
    switch_config activation;
    switch_config repression;
};


struct histone_config
{
    double        bias       = 0;
    double        decay_rate = 0;
    switch_config activation;
    switch_config repression;
};


struct epigene_config
{
    gene_config    gene;
    histone_config histone;
};


struct epigene_state
{
    double potential  = 0;
    int    expression = 0;
};


struct simulation_config
{
    std::vector<epigene_config> epigenes;
    double                      timestep    = 1;
    int                         steps       = 0;
    std::uint64_t               random_seed = 0;
};


class simulation
{
public:
    explicit simulation(simulation_config const& config);
    void     run();

private:
    void   update_deltas();
    double compute_potential_delta(std::size_t gene_index) const;
    int    compute_expression_delta(std::size_t gene_index);
    void   update_state();
    int    poisson(double lambda);
    void   print_state(int step) const;

private:
    simulation_config          m_config;
    std::vector<epigene_state> m_states;
    std::vector<double>        m_potential_deltas;
    std::vector<int>           m_expression_deltas;
    std::mt19937_64            m_random;
};


int main()
{
    // Two mutually repressing genes. The repressing regulations are epigenetic.
    // The simulation shows oscillation between (on, off) and (off, on) states.

    std::vector<epigene_config> const epigenes = {
        {
            .gene = {
                .expression_rate = 0.5,
                .decay_rate      = 0.2,
            },
            .histone = {
                .bias       = 0.5,
                .decay_rate = 0.1,
                .repression = {.gene = 1, .efficiency = 5.0, .rate = 0.8},
            }
        },
        {
            .gene = {
                .expression_rate = 0.5,
                .decay_rate      = 0.2,
            },
            .histone = {
                .bias       = 0.5,
                .decay_rate = 0.1,
                .repression = {.gene = 0, .efficiency = 5.0, .rate = 0.8},
            }
        }
    };

    simulation_config const config = {
        .epigenes    = epigenes,
        .timestep    = 0.1,
        .steps       = 10000,
        .random_seed = 0,
    };

    simulation(config).run();
}


static std::mt19937_64 make_random(std::uint64_t seed)
{
    std::seed_seq seed_seq{seed};
    return std::mt19937_64(seed_seq);
}


static double presense_gate(int x, switch_config const& sw)
{
    auto const weight = sw.efficiency * double(x);
    return sw.rate * weight / (1 + weight);
}


static double absense_gate(int x, switch_config const& sw)
{
    auto const weight = sw.efficiency * double(x);
    return sw.rate / (1 + weight);
}


static double histone_gate(double u)
{
    return 1 / (1 + std::exp(-u));
}


simulation::simulation(simulation_config const& config)
    : m_config(config)
    , m_states(config.epigenes.size())
    , m_potential_deltas(m_states.size())
    , m_expression_deltas(m_states.size())
    , m_random(make_random(config.random_seed))
{
}


void simulation::run()
{
    print_state(0);

    for (int step = 1; step <= m_config.steps; step++) {
        update_deltas();
        update_state();
        print_state(step);
    }
}


void simulation::update_deltas()
{
    for (std::size_t i = 0; i < m_states.size(); i++) {
        m_potential_deltas[i] = compute_potential_delta(i);
        m_expression_deltas[i] = compute_expression_delta(i);
    }
}


double simulation::compute_potential_delta(std::size_t gene_index) const
{
    auto const& histone = m_config.epigenes[gene_index].histone;
    auto const u = m_states[gene_index].potential;
    auto const tau = m_config.timestep;

    // We compute the grow and decay parameters in the differential equation:
    //
    //   du/dt = grow_rate - decay_rate * u .
    //
    // We assume hese parameters to be constant because of the leap condition.
    // Then, we can calculate the analytic solution of u(t + tau) given u(t).
    //
    // grow_rate is modulated by epigenetic activator and epigenetic repressor.
    // decay_rate is constant.

    auto grow_rate = histone.bias;
    auto decay_rate = histone.decay_rate;

    // Activating factor brings up the epigenetic potential.
    if (auto act = histone.activation; act.gene) {
        grow_rate += presense_gate(m_states[*act.gene].expression, act);
    }

    // Repressing factor brings up the epigenetic potential.
    if (auto rep = histone.repression; rep.gene) {
        grow_rate -= presense_gate(m_states[*rep.gene].expression, rep);
    }

    return (u - grow_rate / decay_rate) * std::expm1(-decay_rate * tau);
}


int simulation::compute_expression_delta(std::size_t gene_index)
{
    auto const& gene = m_config.epigenes[gene_index].gene;
    auto const u = m_states[gene_index].potential;
    auto const x = m_states[gene_index].expression;
    auto const tau = m_config.timestep;

    // A single expressed gene may experience two events in the leap time:
    //
    //   (1) Expression.
    //   (2) Decay.
    //
    // (1) may be modulated by histone state, activating protein and repressing
    // protein. We here calcualte the modulated rates, then generate actual
    // number of the events using poisson distribution.

    auto inc_rate = gene.expression_rate;
    auto dec_rate = gene.decay_rate * x;

    // Histone state (on/off) modulates the rate of expression.
    inc_rate *= histone_gate(u);

    // Activating protein brings up the rate on its presense.
    if (auto act = gene.activation; act.gene) {
        inc_rate *= presense_gate(m_states[*act.gene].expression, act);
    }

    // Repressing protein brings down the rate on its presense.
    if (auto rep = gene.repression; rep.gene) {
        inc_rate *= absense_gate(m_states[*rep.gene].expression, rep);
    }

    // Tau-leaping.
    auto const inc = poisson(inc_rate * tau);
    auto const dec = poisson(dec_rate * tau);
    return inc - dec;
}


void simulation::update_state()
{
    for (std::size_t i = 0; i < m_states.size(); i++) {
        m_states[i].potential += m_potential_deltas[i];
        m_states[i].expression += m_expression_deltas[i];

        // Negative expression does not make sense. Saturate at zero.
        m_states[i].expression = std::max(0, m_states[i].expression);
    }
}


int simulation::poisson(double lambda)
{
    std::poisson_distribution<int> distr(lambda);
    return distr(m_random);
}


void simulation::print_state(int step) const
{
    auto const simulated_time = double(step) * m_config.timestep;

    std::cout << simulated_time;

    for (auto const& state : m_states) {
        std::cout
            << '\t'
            << state.potential
            << '\t'
            << state.expression;
    }

    std::cout << '\n';
}
