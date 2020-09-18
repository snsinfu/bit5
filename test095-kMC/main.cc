#include <cstddef>
#include <iostream>
#include <random>

// A ------- B
//   \     /
//    \   /
//     \ /
//      C

enum state_type {
    A, B, C
};

int main()
{
    std::mt19937_64 random(1);
    std::uniform_real_distribution<double> uniform;

    double t = 0;
    state_type state = A;

    const double rates[3][2] = {
        /* A */ {0.5, 0.2},
        /* B */ {0.4, 0.5},
        /* C */ {0.3, 0.2},
    };

    for (long step = 0; step <= 1000; step++) {
        double sum_rate = 0;
        for (int i = 0; i < 2; i++) {
            sum_rate += rates[state][i];
        }

        double stop_rate = uniform(random) * sum_rate;
        double parsum_rate = 0;
        for (int i = 0; i < 2; i++) {
            parsum_rate += rates[state][i];
            if (stop_rate <= parsum_rate) {
                state = state_type((int(state) + i + 1) % 3);
                break;
            }
        }

        double v = uniform(random);
        double dt = -1 / sum_rate * std::log(1 - v);
        t += dt;

        std::cout << t << '\t' << "ABC"[state] << '\n';
    }
}
