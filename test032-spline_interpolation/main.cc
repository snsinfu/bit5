#include <cassert>
#include <cstddef>
#include <iostream>
#include <vector>


static void spline(
    std::vector<double> const& xs,
    std::vector<double> const& ys
);


int main()
{
    std::vector<double> xs = {0, 1, 2, 3, 4, 5};
    std::vector<double> ys = {1, 2, 0, 1, 2, 0};
    spline(xs, ys);
}


static void spline(
    std::vector<double> const& xs,
    std::vector<double> const& ys
)
{
    assert(xs.size() == ys.size());
    assert(xs.size() >= 4);

    // Let us determine the second derivative M of the cubic spline curve f at
    // each segment edge i:
    //
    //      M[i] = f''( x[i] ) ,    i = 0, ..., n - 1 .
    //
    // M is given by the tridiagonal linear equation below:
    //
    //  [ B[0] C[0]                 ] [ M[0]   ]   [ D[0]   ]
    //  [ A[1] B[1] C[1]            ] [ M[1]   ]   [ D[1]   ]
    //  [      A[2] B[2] ..         ] [ M[2]   ] = [ D[2]   ]
    //  [              :  .  C[n-2] ] [   :    ]   [   :    ]
    //  [             A[n-1] B[n-1] ] [ M[n-1] ]   [ D[n-1] ]
    //
    // See: http://fourier.eng.hmc.edu/e176/lectures/ch7/node6.html
    //
    // The subdiagonal A uses 1-based indexing. This simplifies the algorithm.

    auto const n = xs.size();

    std::vector<double> hs(n);
    std::vector<double> As(n);
    std::vector<double> Bs(n);
    std::vector<double> Cs(n);
    std::vector<double> Ds(n);
    std::vector<double> Ms(n);

    for (std::size_t i = 1; i < n; i++) {
        hs[i] = xs[i] - xs[i - 1];
    }

    for (std::size_t i = 1; i < n - 1; i++) {
        Bs[i] = 2;
    }
    Bs[0] = Bs[n - 1] = 1;

    for (std::size_t i = 1; i < n - 1; i++) {
        auto const dx1 = hs[i];
        auto const dx2 = hs[i + 1];
        As[i] = dx1 / (dx1 + dx2);
        Cs[i] = dx2 / (dx1 + dx2);
    }

    for (std::size_t i = 1; i < n - 1; i++) {
        auto const dx1 = hs[i];
        auto const dx2 = hs[i + 1];
        auto const tan1 = (ys[i] - ys[i - 1]) / dx1;
        auto const tan2 = (ys[i + 1] - ys[i]) / dx2;
        Ds[i] = 6 * (tan2 - tan1) / (dx1 + dx2);
    }

    // Solve the tridiagonal linear equation.
    // See: https://en.wikipedia.org/wiki/Tridiagonal_matrix_algorithm

    for (std::size_t i = 1; i < n; i++) {
        auto const w = As[i] / Bs[i - 1];
        Bs[i] -= w * Cs[i - 1];
        Ds[i] -= w * Ds[i - 1];
    }

    Ms[n - 1] = Ds[n - 1] / Bs[n - 1];

    for (std::size_t k = n; k > 0; k--) {
        auto const i = k - 1;
        Ms[i] = (Ds[i] - Cs[i] * Ms[i + 1]) / Bs[i];
    }

    // Now, compute the cubic spline coefficients.
    //
    //  f(x; i) =
    //    (x - xs[i])^3 / (6 hs[i])
    //    + (x - xs[i])^2 / (6 hs[i])
    //    + R1 x
    //    + R0
    //
    // See: http://fourier.eng.hmc.edu/e176/lectures/ch7/node6.html
    std::vector<double> R1(n);
    std::vector<double> R0(n);

    for (std::size_t i = 1; i < n; i++) {
        R1[i] =
            (ys[i] - ys[i - 1]) / hs[i]
            - hs[i] / 6 * (Ms[i] - Ms[i - 1]);

        R0[i] =
            (xs[i] * ys[i - 1] - xs[i - 1] * ys[i]) / hs[i]
            - hs[i] / 6 * (xs[i] * Ms[i - 1] - xs[i - 1] * Ms[i]);
    }


    for (std::size_t i = 1; i < n; i++) {
        std::clog
            << R1[i]
            << '\t'
            << R0[i]
            << '\n';
    }
}
