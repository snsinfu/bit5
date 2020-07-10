// Cubic spline interpolation
#pragma once

#include <vector>


struct cubic_spline
{
    /*
     * Constructs a cubic spline function that passes through given points.
     */
    cubic_spline(std::vector<double> const& x, std::vector<double> const& y);

    /*
     * Evaluates the cubic spline function at `x`.
     */
    double operator()(double x) const;

private:
    std::vector<double> _x;
    std::vector<double> _M;
    std::vector<double> _c;
    std::vector<double> _d;
};
