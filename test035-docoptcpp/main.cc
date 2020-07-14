#include <cassert>
#include <exception>
#include <fstream>
#include <iostream>
#include <map>
#include <stdexcept>
#include <string>

#define DOCOPT_HEADER_ONLY
#include <docopt.h>


static char const* const program_usage = R"(
usage:
  main init <file>
  main show <file>
  main inc [options] <file>

options:
  --amount <num>  specify the amount to increment
)";

using program_options = std::map<std::string, docopt::value>;

static void run_init(program_options const& options);
static void run_show(program_options const& options);
static void run_inc(program_options const& options);

static constexpr int initial_counter = 0;


int main(int argc, char** argv)
{
    try {
        auto const options = docopt::docopt(program_usage, {argv + 1, argv + argc});

        if (options.at("init").asBool()) {
            run_init(options);
        }
        if (options.at("show").asBool()) {
            run_show(options);
        }
        if (options.at("inc").asBool()) {
            run_inc(options);
        }
        return 0;
    } catch (std::exception const& e) {
        std::cerr << "error: " << e.what() << '\n';
        return 1;
    }
    assert(false);
}


static void run_init(program_options const& options)
{
    auto const filename = options.at("<file>").asString();

    std::ofstream file{filename, std::ios::out | std::ios::trunc};
    file << initial_counter << '\n';

    if (!file) {
        throw std::runtime_error("failed to write to counter file");
    }
}


static void run_show(program_options const& options)
{
    auto const filename = options.at("<file>").asString();

    std::ifstream file{filename, std::ios::in};
    int counter;
    file >> counter;

    if (!file) {
        throw std::runtime_error("failed to read from counter file");
    }

    std::cout << counter << '\n';
}


static void run_inc(program_options const& options)
{
    auto const filename = options.at("<file>").asString();

    int amount = 1;
    if (auto const amount_option = options.at("--amount")) {
        amount = int(amount_option.asLong());
    }

    std::fstream file{filename, std::ios::in | std::ios::out};
    int counter;
    file >> counter;

    if (!file) {
        throw std::runtime_error("failed to read from counter file");
    }

    counter += amount;

    file.seekp(0);
    file << counter << '\n';

    if (!file) {
        throw std::runtime_error("failed to write to counter file");
    }
}
