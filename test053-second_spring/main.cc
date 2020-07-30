#include <cmath>
#include <iostream>
#include <random>


int main()
{
    std::mt19937_64 random;

    random.seed(1);
    random.discard(1'000'000);

    double const timestep = 1e-5;
    double const mobility = 1;
    double const temperature = 1;
    double const first_spring = 10.0;
    double const second_spring = 5.0;

    double const weiner_sigma = std::sqrt(2 * timestep * mobility * temperature);
    std::normal_distribution<double> weiner(0, weiner_sigma);

    long const sampling_steps = 1'000'000;
    long const sampling_interval = 100;

    double const x0 = 0;
    double x1 = 0;
    double x2 = 0;
    double W1 = weiner(random);
    double W2 = weiner(random);

    for (long step = 0; step <= sampling_steps; step++) {
        if (step % sampling_interval == 0) {
            std::cout << x1 << '\t' << x2 << '\n';
        }

        double F1 = 0;
        double F2 = 0;

        F1 += -first_spring * (x1 - x0);
        F1 += -first_spring * (x1 - x2);
        F2 += -first_spring * (x2 - x1);
        F2 += -second_spring * (x2 - x0);

        double const new_W1 = weiner(random);
        double const new_W2 = weiner(random);

        x1 += timestep * mobility * F1 + (W1 + new_W1) / 2;
        x2 += timestep * mobility * F2 + (W2 + new_W2) / 2;

        W1 = new_W1;
        W2 = new_W2;
    }
}
