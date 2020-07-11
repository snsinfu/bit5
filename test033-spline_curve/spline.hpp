// Cubic spline interpolation
#pragma once

#include <cassert>
#include <vector>


class cubic_spline
{
    static constexpr int order = 3;

    struct spline_data {
        double knot = 0;
        double coefficients[order + 1] = {};
    };

public:
    /*
     * Constructs a cubic spline function that passes through given points.
     */
    cubic_spline(std::vector<double> const& t, std::vector<double> const& x);

    /*
     * Evaluates the cubic spline function at `t`.
     */
    double operator()(double t) const;

private:
    spline_data const& find_spline(double t) const;

private:
    std::vector<spline_data> _splines;
};


inline
cubic_spline::cubic_spline(std::vector<double> const& t, std::vector<double> const& x)
{
    using index_t = std::vector<spline_data>::size_type;

    assert(t.size() == x.size());
    assert(t.size() >= 2);

    index_t const n = t.size();

    std::vector<double> a = x;
    std::vector<double> b(n);
    std::vector<double> c(n);
    std::vector<double> d(n);

    std::vector<double> alpha(n);
    std::vector<double> lambda(n);
    std::vector<double> mu(n);
    std::vector<double> zeta(n);

    for (index_t i = 1; i < n - 1; i++) {
        auto const dx1 = x[i + 1] - x[i];
        auto const dt1 = t[i + 1] - t[i];
        auto const dx2 = x[i] - x[i - 1];
        auto const dt2 = t[i] - t[i - 1];
        alpha[i] = 3 * (dx1 / dt1 - dx2 / dt2);
    }

    lambda[0] = lambda[n - 1] = 1;

    for (index_t i = 1; i < n - 1; i++) {
        auto const dt1 = t[i + 1] - t[i];
        auto const dt2 = t[i] - t[i - 1];
        lambda[i] = 2 * (dt1 + dt2) - dt2 * mu[i - 1];
        mu[i] = dt1 / lambda[i];
        zeta[i] = (alpha[i] - dt2 * zeta[i - 1]) / lambda[i];
    }

    for (index_t i_plus = n - 1; i_plus > 0; i_plus--) {
        index_t const i = i_plus - 1;

        auto const dx = x[i + 1] - x[i];
        auto const dt = t[i + 1] - t[i];

        c[i] = zeta[i] - mu[i] * c[i + 1];
        b[i] = dx / dt - dt * (c[i + 1] + 2 * c[i]) / 3;
        d[i] = (c[i + 1] - c[i]) / dt;
    }

    for (index_t i = 0; i < n - 1; i++) {
        spline_data spline;
        spline.knot = t[i];
        spline.coefficients[0] = a[i];
        spline.coefficients[1] = b[i];
        spline.coefficients[2] = c[i];
        spline.coefficients[3] = d[i];
        _splines.push_back(spline);
    }
    _splines.shrink_to_fit();
}

inline double
cubic_spline::operator()(double t) const
{
    auto const& spline = find_spline(t);

    auto value = spline.coefficients[order];

    for (int i = order - 1; i >= 0; i--) {
        value *= t - spline.knot;
        value += spline.coefficients[i];
    }

    return value;
}

inline cubic_spline::spline_data const&
cubic_spline::find_spline(double t) const
{
    using index_t = std::vector<spline_data>::size_type;

    index_t index = 0;
    for (; index + 1 < _splines.size(); index++) {
        if (t < _splines[index + 1].knot) {
            break;
        }
    }
    return _splines[index];
}
