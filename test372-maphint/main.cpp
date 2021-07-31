#include <algorithm>
#include <cassert>
#include <chrono>
#include <iostream>
#include <string>
#include <map>


// Using std::map hint on insertion. It gave ~2x speedup on my laptop.

static std::string make_id(long seq);


int main()
{
    using clock = std::chrono::high_resolution_clock;

    constexpr bool with_hint = true;
    double dummy = 0;
    long const bench_size = 100000;
    long const bench_repeat = 100;
    auto const start_time = clock::now();

    for (long rep = 0; rep < bench_repeat; rep++) {
        std::map<std::string, double> data;
        auto hint = data.end();

        for (long i = 0; i < bench_size; i++) {
            auto const id = make_id(i);
            auto const value = 0.1 * i;

            if (with_hint) {
                hint = data.insert(hint, {id, value});
            } else {
                data.insert({id, value});
            }
        }

        auto const pos = data.find("IDnonexistent");
        if (pos != data.end()) {
            dummy += pos->second;
        }
    }

    auto const end_time = clock::now();
    auto const elapsed_time = end_time - start_time;
    auto const seconds = std::chrono::duration<double>(elapsed_time).count();
    auto const op_seconds = seconds / bench_repeat / bench_size;

    std::cout << op_seconds * 1e9 << " ns/op\n";
    std::cout << "dummy = " << dummy << '\n';
}


std::string make_id(long seq)
{
    // We don't have <format> yet...
    assert(seq >= 0);
    assert(seq < 10000000000);
    std::string id{"ID0000000000"};
    auto const digits = std::to_string(seq);
    std::copy_backward(digits.begin(), digits.end(), id.end());
    return id;
}
