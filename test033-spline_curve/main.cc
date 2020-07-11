// 3D spline

#include <iostream>
#include <vector>

#include "spline.hpp"


int main()
{
    // Five sample points from a circle

    std::vector<double> const ts = {
        0.5, 1.5, 2.5, 3.5, 4.5
    };
    std::vector<double> const xs = {
        -1, -0.7071, 0, 0.7071, 1
    };
    std::vector<double> const ys = {
        0, 0.7071, 1, 0.7071, 0
    };

    // 2x supersampling with extrapolation

    auto const spline_x = cubic_spline(ts, xs);
    auto const spline_y = cubic_spline(ts, ys);

    std::vector<double> const sampling_t = {
        0, 0.5, 1, 1.5, 2, 2.5, 3, 3.5, 4, 4.5, 5
    };

    for (auto const st : sampling_t) {
        auto const sx = spline_x(st);
        auto const sy = spline_y(st);
        std::cout << st << '\t' << sx << '\t' << sy << '\n';
    }
}
