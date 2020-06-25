#include <cmath>
#include <initializer_list>
#include <iostream>

static double comb(std::initializer_list<int> mults);
static double fact(int n);

int main()
{
    int const N = 3;

    double states = 0;
    for (int pairs = 0; pairs <= N / 2; pairs++) {
        states += comb({
            N - 2 * pairs,
            pairs,
            pairs,
            N
        });
    }

    double domain_states = 0;
    for (int pairs = 0; pairs <= N / 2; pairs++) {
        for (int pores = 0; pores <= pairs; pores++) {
            auto const outer = comb({
                N - 2 * pairs,
                pairs,
                pairs - pores,
                1
            });
            auto const inner = comb({
                N - 2,
                pores
            });
            domain_states += outer * inner;
        }
    }

    std::cout << "W: " << states << '\n';
    std::cout << "W(domain): " << domain_states << '\n';
}

static double comb(std::initializer_list<int> mults)
{
    double prod = 1;
    int sum = 0;
    for (int mult : mults) {
        prod *= fact(mult);
        sum += mult;
    }
    return fact(sum) / prod;
}

static double fact(int n)
{
    return std::tgamma(static_cast<double>(n + 1));
}
