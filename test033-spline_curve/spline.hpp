// Cubic spline interpolation
#pragma once

#include <cassert>
#include <cstddef>
#include <vector>


/*
 * Cubic spline function that interpolates one-dimensional knots.
 */
class cubic_spline
{
    static constexpr int order = 3;

    struct spline_data {
        double knot = 0;
        double coefficients[order + 1] = {};
    };

public:
    /*
     * Constructs a cubic spline function that passes through given knots.
     *
     * @param t     Specify knot points. This must be sorted.
     * @param x     Specify values at the knot points.
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

namespace detail_cubic_spline {
    /*
     * Solves a tridiagonal system of equations. This function clobbers given
     * coefficient vectors.
     */
    void solve_tridiagonal_system(
        std::vector<double>& lower,
        std::vector<double>& diag,
        std::vector<double>& upper,
        std::vector<double>& rhs,
        std::vector<double>& solution
    );
}

inline
cubic_spline::cubic_spline(
    std::vector<double> const& knots,
    std::vector<double> const& values
)
{
    assert(knots.size() == values.size());

    auto const n_knots = knots.size();
    auto const n_segments = n_knots - 1;

    std::vector<double> intervals(n_segments);
    std::vector<double> slopes(n_segments);

    for (std::size_t i = 0; i < n_segments; i++) {
        auto const dt = knots[i + 1] - knots[i];
        auto const dx = values[i + 1] - values[i];
        intervals[i] = dt;
        slopes[i] = dx / dt;
    }

    // Let M[i] be the second derivative of the i-th spline at the i-th knot,
    // i = 0,...,n where n is the number of segments. The vector M is given by
    // a tridiagonal system of equations:
    //
    // [ D[0] U[0]                      ] [ M[0]   ]   [ Y[0]   ]
    // [ L[1] D[1] U[1]                 ] [ M[1]   ]   [ Y[1]   ]
    // [      L[2] D[2] U[2]            ] [ M[2]   ]   [ Y[2]   ]
    // [      ...  ...  ...             ] [   :    ] = [   :    ]
    // [           L[n-1] D[n-1] U[n-1] ] [ M[n-1] ]   [ Y[n-1] ]
    // [                  L[n]   D[n]   ] [ M[n]   ]   [ Y[n]   ]
    //
    // We calculate the coefficients L, D, U and Y below.

    std::vector<double> L(n_segments + 1);
    std::vector<double> D(n_segments + 1);
    std::vector<double> U(n_segments + 1);
    std::vector<double> Y(n_segments + 1);
    std::vector<double> M(n_segments + 1);

    D[0] = 1;
    D[n_segments] = 1;

    for (std::size_t i = 1; i < n_segments; i++) {
        L[i] = intervals[i - 1];
        D[i] = 2 * (intervals[i - 1] + intervals[i]);
        U[i] = intervals[i];
        Y[i] = 6 * (slopes[i] - slopes[i - 1]);
    }

    detail_cubic_spline::solve_tridiagonal_system(L, D, U, Y, M);

    assert(M[0] == 0);
    assert(M[n_segments] == 0);

    // Derive the polynomial coefficients of each spline segment from the
    // second derivatives we obtained.

    for (std::size_t i = 0; i < n_segments; i++) {
        spline_data spline;
        spline.knot = knots[i];
        spline.coefficients[0] = values[i];
        spline.coefficients[1] = slopes[i] - (M[i + 1] + 2 * M[i]) * intervals[i] / 6;
        spline.coefficients[2] = M[i] / 2;
        spline.coefficients[3] = (M[i + 1] - M[i]) / (6 * intervals[i]);
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
    // FIXME: Inefficient.

    std::size_t index = 0;
    for (; index + 1 < _splines.size(); index++) {
        if (t < _splines[index + 1].knot) {
            break;
        }
    }
    return _splines[index];
}

inline void
detail_cubic_spline::solve_tridiagonal_system(
    std::vector<double>& lower,
    std::vector<double>& diag,
    std::vector<double>& upper,
    std::vector<double>& rhs,
    std::vector<double>& solution
)
{
    // See: https://en.wikipedia.org/wiki/Tridiagonal_matrix_algorithm

    std::size_t const dim = rhs.size();

    assert(lower.size() == dim);
    assert(diag.size() == dim);
    assert(upper.size() == dim);
    assert(solution.size() == dim);

    for (std::size_t i = 1; i < dim; i++) {
        auto const w = lower[i] / diag[i - 1];
        diag[i] -= w * upper[i - 1];
        rhs[i] -= w * rhs[i - 1];
    }

    solution[dim - 1] = rhs[dim - 1] / diag[dim - 1];

    for (std::size_t i_plus = dim; i_plus > 0; i_plus--) {
        auto const i = i_plus - 1;
        solution[i] = (rhs[i] - upper[i] * solution[i + 1]) / diag[i];
    }
}
