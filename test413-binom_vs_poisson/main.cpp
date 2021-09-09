#include <chrono>
#include <iostream>
#include <random>
#include <string>


// Performance comparison of generation of binomial and poisson random numbers.
//
// Result (libc++):
// binomial:       6.74735 ms/task sum: 448500
// poisson:        1.8017 ms/task  sum: 449700
//
// Binomial numbers are significantly (~4x) slower to generate than poisson
// numbers.


static void benchmark(std::string const& label, int(* task)(int), int size, int repeats);
static int  binomial_task(int size);
static int  poisson_task(int size);

using random_type = std::mt19937_64;


int main()
{
    constexpr int task_size = 100000;
    constexpr int repeats = 100;

    benchmark("binomial", binomial_task, task_size, repeats);
    benchmark("poisson", poisson_task, task_size, repeats);
}


void benchmark(std::string const& label, int(* task)(int), int size, int repeats)
{
    using clock = std::chrono::high_resolution_clock;

    auto const start = clock::now();

    int sum = 0;
    for (int rep = 0; rep < repeats; rep++) {
        sum += task(size);
    }

    auto const end = clock::now();
    auto const seconds = std::chrono::duration<double>(end - start).count();
    auto const seconds_per_task = seconds / double(repeats);

    std::cout
        << label
        << ":\t"
        << seconds_per_task * 1e3
        << " ms/task"
        << '\t'
        << "sum: "
        << sum
        << '\n';
}


int binomial_task(int size)
{
    random_type random;
    int sum = 0;

    for (int i = 0; i < size; i++) {
        auto const n = i % 10;
        auto const p = 0.01;
        std::binomial_distribution<int> binomial{n, p};
        sum += binomial(random);
    }

    return sum;
}


int poisson_task(int size)
{
    random_type random;
    int sum = 0;

    for (int i = 0; i < size; i++) {
        auto const mean = (i % 10) * 0.01;
        std::poisson_distribution<int> poisson{mean};
        sum += poisson(random);
    }

    return sum;
}
