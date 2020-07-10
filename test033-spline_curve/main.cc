// 3D spline

#include <cassert>
#include <iostream>
#include <vector>

#include "vec.hpp"
#include "spline.hpp"


int main()
{
    // Define the input points

    std::vector<vec> const points = {
        { 0, 0, 0 },
        { 1, 0, 0 },
        { 0, 1, 0 },
        { 0, 0, 1 }
    };
    std::vector<double> const chord = {
        0, 1, 2, 3
    };
    assert(points.size() == chord.size());

    //
}
