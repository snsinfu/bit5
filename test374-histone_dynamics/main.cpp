#include <cstddef>
#include <iostream>
#include <numeric>
#include <random>
#include <vector>


//         { σ(HATs) - σ(HDACs) - ku    (u > 0)
// du/dt = {
//         { σ(EZH2) - σ(KDMs) - ku     (u < 0)


static double mean(std::vector<int> const& vec);
static int    sign(int x);


int main()
{
    constexpr double      time_delta = 1;
    constexpr long        n_steps    = 1000;
    constexpr std::size_t n_sites    = 16;

    std::mt19937_64 random;

    auto const tau_leap = [&](double rate) {
        std::poisson_distribution<int> distr{rate * time_delta};
        return distr(random);
    };

    std::vector<int> epi_state(n_sites);

    for (long step = 0; step < n_steps; step++) {

        auto const mean_state = mean(epi_state);

        double const ac_write_rate = (mean_state >= 0) * 0.2;
        double const ac_erase_rate = (mean_state >= 0) * 0.1;
        double const me_write_rate = (mean_state <= 0) * 0.2;
        double const me_erase_rate = (mean_state <= 0) * 0.1;
        double const neutral_rate = 0.1;

        for (std::size_t i = 0; i < n_sites; i++) {
            auto const ac_write = tau_leap(ac_write_rate);
            auto const ac_erase = tau_leap(ac_erase_rate);
            auto const me_write = tau_leap(me_write_rate);
            auto const me_erase = tau_leap(me_erase_rate);
            auto const neutral = tau_leap(neutral_rate);

            epi_state[i] -= epi_state[i] * neutral;
            epi_state[i] += ac_write - ac_erase - me_write + me_erase;
            epi_state[i] = sign(epi_state[i]);
        }

        for (std::size_t i = 0; i < n_sites; i++) {
            char const ch =
                epi_state[i] < 0 ? '-' :
                epi_state[i] > 0 ? '+' : '.';
            std::cout << ch;
        }

        auto const state = mean(epi_state);

        std::cout
            << '\t'
            << (state < -0.2 ? 'r' : state < 0.2 ? '.' : 'A')
            << '\n';
    }
}

double mean(std::vector<int> const& vec)
{
    return std::accumulate(vec.begin(), vec.end(), 0) / double(vec.size());
}

int sign(int x)
{
    return
        x < 0 ? -1 :
        x > 0 ? +1 : 0;
}
