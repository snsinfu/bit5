// 1D diffusion with collision.

#include <cassert>
#include <cstddef>
#include <cstdint>
#include <iostream>
#include <random>
#include <string>
#include <vector>


using std::int8_t;
using std::size_t;


struct loop_pair
{
    size_t start  = 0;
    size_t end    = 0;
    bool   loaded = false;
};

namespace site_mod
{
    enum : uint8_t
    {
        none = 0,
        occupied = 1,
        loading_site = 2,
        unloading_site = 4,
    };
};


int main()
{
    constexpr size_t n_sites  = 50;
    constexpr size_t n_loops  = 50;
    constexpr long   steps    = 100000;
    constexpr double dt       = 0.01;
    constexpr long   interval = 100;

    constexpr double boundary_speed = 0.2;
    constexpr double loading_rate   = 0.01;
    constexpr double unloading_rate = 0.1;
    constexpr double forward_rate   = 1.0;
    constexpr double backward_rate  = 1.0;

    // For events to be effectively independent we need to assume sufficiently
    // low probability of the events.
    assert(loading_rate * dt < 0.1);
    assert(unloading_rate * dt < 0.1);
    assert(forward_rate * dt < 0.1);
    assert(backward_rate * dt < 0.1);

    std::vector<size_t> const loading_sites = {
        10, 20, 30
    };
    std::vector<size_t> const unloading_sites = {
        1, 48
    };

    std::mt19937_64 random;

    auto const bernoulli = [&](double prob) {
        std::uniform_real_distribution<double> uniform;
        return uniform(random) < prob;
    };

    std::vector<double>    speed(n_sites, 1);
    std::vector<uint8_t>   sites(n_sites);
    std::vector<loop_pair> loops(n_loops);

    // Boundaries
    sites.front() |= site_mod::occupied;
    speed.front() *= boundary_speed;
    sites.back() |= site_mod::occupied;
    speed.back() *= boundary_speed;

    // (Un)Loading sites
    for (auto const& pos : loading_sites) {
        sites[pos] |= site_mod::loading_site;
    }

    for (auto const& pos : unloading_sites) {
        sites[pos] |= site_mod::unloading_site;
    }

    auto const load_loop = [&](size_t pos) {
        if ((sites[pos] | sites[pos + 1]) & site_mod::occupied) {
            return;
        }

        for (auto& loop : loops) {
            if (!loop.loaded) {
                loop.start = pos;
                loop.end = pos + 1;
                loop.loaded = true;
                sites[loop.start] |= site_mod::occupied;
                sites[loop.end] |= site_mod::occupied;
                break;
            }
        }
    };

    for (long step = 0; step < steps; step++) {
        // Loading
        for (auto const pos : loading_sites) {
            if (bernoulli(loading_rate * dt)) {
                load_loop(pos);
            }
        }

        // Diffusion and unloading
        for (auto& loop : loops) {
            if (!loop.loaded) {
                continue;
            }

            if ((sites[loop.start] | sites[loop.end]) & site_mod::unloading_site) {
                if (bernoulli(unloading_rate * dt)) {
                    sites[loop.start] &= ~site_mod::occupied;
                    sites[loop.end] &= ~site_mod::occupied;
                    loop.loaded = false;
                    continue;
                }
            }

            // Start ring
            if (bernoulli(speed[loop.start] * forward_rate * dt)) {
                if (!(sites[loop.start - 1] & site_mod::occupied)) {
                    sites[loop.start] &= ~site_mod::occupied;
                    loop.start--;
                    sites[loop.start] |= site_mod::occupied;
                }
            }

            if (bernoulli(speed[loop.start] * backward_rate * dt)) {
                if (!(sites[loop.start + 1] & site_mod::occupied)) {
                    sites[loop.start] &= ~site_mod::occupied;
                    loop.start++;
                    sites[loop.start] |= site_mod::occupied;
                }
            }

            // End ring
            if (bernoulli(speed[loop.end] * forward_rate * dt)) {
                if (!(sites[loop.end + 1] & site_mod::occupied)) {
                    sites[loop.end] &= ~site_mod::occupied;
                    loop.end++;
                    sites[loop.end] |= site_mod::occupied;
                }
            }

            if (bernoulli(speed[loop.end] * backward_rate * dt)) {
                if (!(sites[loop.end - 1] & site_mod::occupied)) {
                    sites[loop.end] &= ~site_mod::occupied;
                    loop.end--;
                    sites[loop.end] |= site_mod::occupied;
                }
            }

            if (loop.end < loop.start) {
                std::swap(loop.start, loop.end);
            }
        }

        if (step % interval == 0) {
            std::string text(n_sites, ' ');

            for (auto const& loop : loops) {
                if (loop.loaded) {
                    text[loop.start] = '<';
                    text[loop.end] = '>';
                }
            }

            text.front() = '|';
            text.back() = '|';

            std::cout << text << '\n';
        }
    }
}
