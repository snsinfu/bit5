#include <algorithm>
#include <cstddef>
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

template<typename RNG>
static int poisson(double lambda, RNG& random);


int main()
{
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

    double const tau = 0.1;
    int const steps = 10000;

    std::vector<epigene_state> states(epigenes.size());
    std::mt19937_64 random;

    std::vector<double> potential_deltas(epigenes.size());
    std::vector<int> expression_deltas(epigenes.size());

    for (int step = 1; step <= steps; step++) {
        for (std::size_t i = 0; i < epigenes.size(); i++) {
            auto const& gene = epigenes[i].gene;
            auto const& histone = epigenes[i].histone;

            // Histone dynamics
            auto const u = states[i].potential;

            double grow_rate = histone.bias;
            double decay_rate = histone.decay_rate;

            if (auto act = histone.activation; act.gene) {
                auto const ex = act.efficiency * states[*act.gene].expression;
                grow_rate += act.rate * ex / (1 + ex);
            }

            if (auto rep = histone.repression; rep.gene) {
                auto const ex = rep.efficiency * states[*rep.gene].expression;
                grow_rate -= rep.rate * ex / (1 + ex);
            }

            potential_deltas[i] = (u - grow_rate / decay_rate) * std::expm1(-decay_rate * tau);

            // Expression dynamics
            int dx = 0;

            double rate = gene.expression_rate;

            rate *= 1 / (1 + std::exp(-u));

            if (auto act = gene.activation; act.gene) {
                auto const ex = act.efficiency * states[*act.gene].expression;
                rate *= act.rate * ex / (1 + ex);
            }

            if (auto rep = gene.repression; rep.gene) {
                auto const ex = rep.efficiency * states[*rep.gene].expression;
                rate *= rep.rate / (1 + ex);
            }

            dx += poisson(rate * tau, random);
            dx -= poisson(gene.decay_rate * states[i].expression * tau, random);

            expression_deltas[i] = dx;
        }

        for (std::size_t i = 0; i < epigenes.size(); i++) {
            states[i].potential += potential_deltas[i];
            states[i].expression += expression_deltas[i];

            if (states[i].expression < 0) {
                states[i].expression = 0;
            }
        }

        std::cout << tau * double(step);
        for (auto const& state : states) {
            std::cout
                << '\t'
                << state.potential
                << '\t'
                << state.expression;
        }
        std::cout << '\n';
    }
}


template<typename RNG>
int poisson(double lambda, RNG& random)
{
    std::poisson_distribution<int> distr(lambda);
    return distr(random);
}
