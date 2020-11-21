// Discrete Langevin dynamics. https://arxiv.org/abs/1901.05214

#include <cmath>
#include <iostream>
#include <random>
#include <tuple>


namespace
{
    double normal_pdf(double x)
    {
        double const normalizer = std::sqrt(1 / (2 * 3.1415926535));
        return normalizer * std::exp(-0.5 * x * x);
    }

    double normal_cdf(double x)
    {
        double const scaler = std::sqrt(0.5);
        return 0.5 * (1 + std::erf(scaler * x));
    }
}


int main()
{
    int const simulation_steps = 100;
    double const stepsize = 0.5;
    double const beta = 0.1;
    double const epsilon = 2 * stepsize / beta;
    double const sqrt_epsilon = std::sqrt(epsilon);
    double const invsqrt_epsilon = 1 / sqrt_epsilon;
    double const lambda = sqrt_epsilon * normal_pdf(-invsqrt_epsilon) / normal_cdf(-invsqrt_epsilon);

    std::mt19937_64 random;
    std::normal_distribution<double> normal;
    std::uniform_int_distribution<int> zero_one(0, 1);

    int x = 0;

    auto const propose = [&] {
        int const next_x = x + 1 - 2 * zero_one(random);
        double const delta_energy = -0.1 * (next_x - x);
        return std::make_tuple(next_x, delta_energy);
    };

    std::cout << x;

    for (int step = 1; step <= simulation_steps; step++) {
        auto const [next_x, delta_energy] = propose();
        double const delta_s = 0.5 * beta * delta_energy;
        double const condition = -1 - epsilon / (2 * lambda) * delta_s + sqrt_epsilon * normal(random);

        if (condition > 0) {
            x = next_x;
        }

        std::cout << ' ' << x;
    }

    std::cout << '\n';

    // Something is wrong. The trajectory is too much biased toward positive
    // direction despite the high temperature (small beta). Also, making the
    // energy slope (the coefficient of delta_energy) negative does not
    // change the positive bias. That behavior is absolutely wrong!
}
