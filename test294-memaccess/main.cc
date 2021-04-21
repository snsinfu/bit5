#include <chrono>
#include <cstddef>
#include <iostream>
#include <random>
#include <vector>


struct bench_config
{
    std::size_t rows;
    std::size_t cols;
    long iter_count;
};

static void run_bench(double(& bench)(bench_config const& config));
static double bench_vector_of_vectors(bench_config const& config);
static double bench_row_major(bench_config const& config);
static double bench_col_major(bench_config const& config);


int main()
{
    run_bench(bench_vector_of_vectors);
    run_bench(bench_row_major);
    run_bench(bench_col_major);
}

void run_bench(double(& bench)(bench_config const&))
{
    using clock = std::chrono::steady_clock;

    bench_config const config = {
        10000000, 10, 100
    };

    auto const start_time = clock::now();
    auto const result = bench(config);
    auto const end_time = clock::now();
    auto const seconds = std::chrono::duration<double>(end_time - start_time).count();
    auto const ms_per_iteration = 1000.0 * seconds / double(config.iter_count);

    std::cout << result << '\t' << ms_per_iteration << '\n';
}

double bench_vector_of_vectors(bench_config const& config)
{
    auto const rows = config.rows;
    auto const cols = config.cols;
    auto const iter_count = config.iter_count;

    std::vector<std::vector<double>> matrix(rows, std::vector<double>(cols, 0.0));

    std::mt19937_64 random;
    std::uniform_real_distribution<double> uniform{-1.0, 1.0};

    for (std::size_t i = 0; i < rows; ++i) {
        for (std::size_t j = 0; j < cols; ++j) {
            matrix[i][j] = uniform(random);
        }
    }

    std::size_t const column = cols / 2;
    double result = 0;

    for (long iter = 0; iter < iter_count; ++iter) {
        for (std::size_t i = 0; i < rows; ++i) {
            result += matrix[i][column];
        }
    }

    return result;
}

double bench_row_major(bench_config const& config)
{
    auto const rows = config.rows;
    auto const cols = config.cols;
    auto const iter_count = config.iter_count;

    std::vector<double> matrix(rows * cols, 0.0);

    std::mt19937_64 random;
    std::uniform_real_distribution<double> uniform{-1.0, 1.0};

    for (std::size_t i = 0; i < rows; ++i) {
        for (std::size_t j = 0; j < cols; ++j) {
            matrix[i * cols + j] = uniform(random);
        }
    }

    auto const column = cols / 2;
    double result = 0;

    for (long iter = 0; iter < iter_count; ++iter) {
        for (std::size_t i = 0; i < rows; ++i) {
            result += matrix[i * cols + column];
        }
    }

    return result;
}

double bench_col_major(bench_config const& config)
{
    auto const rows = config.rows;
    auto const cols = config.cols;
    auto const iter_count = config.iter_count;

    std::vector<double> matrix(rows * cols, 0.0);

    std::mt19937_64 random;
    std::uniform_real_distribution<double> uniform{-1.0, 1.0};

    for (std::size_t i = 0; i < rows; ++i) {
        for (std::size_t j = 0; j < cols; ++j) {
            matrix[i + j * rows] = uniform(random);
        }
    }

    auto const column = cols / 2;
    double result = 0;

    for (long iter = 0; iter < iter_count; ++iter) {
        for (std::size_t i = 0; i < rows; ++i) {
            result += matrix[i + column * rows];
        }
    }

    return result;
}
