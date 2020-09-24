#pragma once

#include <cmath>

#include <md.hpp>


/*
 * u(r) = e (1 - r/r0)^N
 * F(r) = N e (1 - r/r0)^(N-1) / r0^2 / (r/r0)
 * for r < r0
 */
template<int N>
struct power_potential
{
    md::scalar energy = 1;
    md::scalar cutoff_distance = 1;

    inline md::scalar evaluate_energy(md::vector r) const
    {
        md::scalar const k2 = 1 / (cutoff_distance * cutoff_distance);
        md::scalar const u2 = k2 * r.squared_norm();

        if (u2 > 1) {
            return 0;
        }

        return energy * md::power<N>(1 - std::sqrt(u2));
    }

    inline md::vector evaluate_force(md::vector r) const
    {
        constexpr md::scalar u_epsilon = 1e-6;

        md::scalar const k2 = 1 / (cutoff_distance * cutoff_distance);
        md::scalar const u2 = k2 * r.squared_norm();

        if (u2 > 1) {
            return {};
        }

        md::scalar const u1 = std::sqrt(u2);
        md::scalar const inv_u = 1 / (u1 + u_epsilon);
        md::scalar const fdivr = N * energy * k2 * inv_u * md::power<N - 1>(1 - u1);

        return fdivr * r;
    }
};
