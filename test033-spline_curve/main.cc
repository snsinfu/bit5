// 3D spline

#include <cassert>
#include <iostream>
#include <vector>

#include "vec.hpp"
#include "spline.hpp"


int main()
{
    std::vector<double> const t = {0, 1, 2, 3, 4, 5};
    std::vector<double> const x = {1, 2, 3, 2, 1, 2};

    auto const spline = cubic_spline(t, x);

    std::vector<double> const sampling_t = {
        0, 0.5, 1, 1.5, 2, 2.5, 3, 3.5, 4, 4.5, 5, 5.5
    };
    for (auto const st : sampling_t) {
        std::cout << st << '\t' << spline(st) << '\n';
    }
}
