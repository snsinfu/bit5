// Bond Fluctuation Model

#include <cmath>
#include <cstddef>
#include <cstdint>
#include <iostream>
#include <random>
#include <vector>

#include "cubic_lattice.hpp"
#include "maybe_index.hpp"


struct lattice_state
{
    maybe_index<std::uint16_t> monomer;
};

struct monomer_state
{
    std::int32_t x = 0;
    std::int32_t y = 0;
    std::int32_t z = 0;
};


int main()
{
    cubic_lattice<lattice_state> lattice(4);
    std::vector<monomer_state> monomers;
    std::mt19937_64 random;


    auto occupy = [&](std::uint16_t i, std::int32_t x, std::int32_t y, std::int32_t z) -> bool {
        lattice_state* sites[] = {
            &lattice(x + 0, y + 0, z + 0),
            &lattice(x + 1, y + 0, z + 0),
            &lattice(x + 0, y + 1, z + 0),
            &lattice(x + 1, y + 1, z + 0),
            &lattice(x + 0, y + 0, z + 1),
            &lattice(x + 1, y + 0, z + 1),
            &lattice(x + 0, y + 1, z + 1),
            &lattice(x + 1, y + 1, z + 1),
        };

        for (auto site : sites) {
            if (site->monomer) {
                return false;
            }
        }

        for (auto site : sites) {
            site->monomer = i;
        }

        return true;
    };

    auto release = [&](std::int32_t x, std::int32_t y, std::int32_t z) {
        lattice_state* sites[] = {
            &lattice(x + 0, y + 0, z + 0),
            &lattice(x + 1, y + 0, z + 0),
            &lattice(x + 0, y + 1, z + 0),
            &lattice(x + 1, y + 1, z + 0),
            &lattice(x + 0, y + 0, z + 1),
            &lattice(x + 1, y + 0, z + 1),
            &lattice(x + 0, y + 1, z + 1),
            &lattice(x + 1, y + 1, z + 1),
        };

        for (auto site : sites) {
            site->monomer.reset();
        }
    };


    // Initialization
    {
        // bond_mask[dx][dy][dz] is true if bond vector (dx,dy,dz) is permitted
        // per the elementary bond set proposed by Deutsch (1991).
        bool const bond_mask[4][4][4] = {
            {{0, 0, 1, 1}, {0, 0, 1, 1}, {1, 1, 0, 0}, {1, 1, 0, 0}},
            {{0, 0, 1, 1}, {0, 0, 1, 0}, {1, 1, 1, 0}, {1, 0, 0, 0}},
            {{1, 1, 0, 0}, {1, 1, 1, 0}, {0, 1, 0, 0}, {0, 0, 0, 0}},
            {{1, 1, 0, 0}, {1, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}},
        };

        std::int32_t x = 0;
        std::int32_t y = 0;
        std::int32_t z = 0;

        std::uniform_int_distribution<std::int32_t> delta{-4, 4};

        for (std::uint16_t i = 0; i < 200; i++) {
            auto const prev_x = x;
            auto const prev_y = y;
            auto const prev_z = z;
            for (;;) {
                x = prev_x + delta(random);
                y = prev_y + delta(random);
                z = prev_z + delta(random);

                auto const already_occupied =
                    lattice(x + 0, y + 0, z + 0).monomer ||
                    lattice(x + 0, y + 0, z + 1).monomer ||
                    lattice(x + 0, y + 1, z + 0).monomer ||
                    lattice(x + 0, y + 1, z + 1).monomer ||
                    lattice(x + 1, y + 0, z + 0).monomer ||
                    lattice(x + 1, y + 0, z + 1).monomer ||
                    lattice(x + 1, y + 1, z + 0).monomer ||
                    lattice(x + 1, y + 1, z + 1).monomer;

                if (already_occupied) {
                    continue;
                }

                auto const dx = std::abs(x - prev_x);
                auto const dy = std::abs(y - prev_y);
                auto const dz = std::abs(z - prev_z);

                if (bond_mask[dx][dy][dz]) {
                    break;
                }
            }

            lattice(x + 0, y + 0, z + 0) = {.monomer = i};
            lattice(x + 0, y + 0, z + 1) = {.monomer = i};
            lattice(x + 0, y + 1, z + 0) = {.monomer = i};
            lattice(x + 0, y + 1, z + 1) = {.monomer = i};
            lattice(x + 1, y + 0, z + 0) = {.monomer = i};
            lattice(x + 1, y + 0, z + 1) = {.monomer = i};
            lattice(x + 1, y + 1, z + 0) = {.monomer = i};
            lattice(x + 1, y + 1, z + 1) = {.monomer = i};

            monomers.push_back({
                .x = x,
                .y = y,
                .z = z,
            });
        }
    }

    // Simulation
    long accepts = 0;

    for (long step = 0; step < 100000; step++)
    {
        auto const n_monomers = std::uint16_t(monomers.size());

        std::int32_t const min_bond_sq = 4;
        std::int32_t const max_bond_sq = 10;

        std::int32_t const dx_set[] = {1, -1, 0, 0, 0, 0};
        std::int32_t const dy_set[] = {0, 0, 1, -1, 0, 0};
        std::int32_t const dz_set[] = {0, 0, 0, 0, 1, -1};

        for (std::uint16_t i = 0; i < n_monomers; i++) {
            std::uniform_int_distribution<std::uint16_t> monomer_choice{0, std::uint16_t(n_monomers - 1)};
            std::uniform_int_distribution<std::size_t> direction_choice{0, 5};

            auto const index = monomer_choice(random);
            auto const direction = direction_choice(random);

            auto& monomer = monomers[index];

            auto const x = monomer.x + dx_set[direction];
            auto const y = monomer.y + dy_set[direction];
            auto const z = monomer.z + dz_set[direction];

            // Check bond length
            if (index > 0) {
                auto const dx = x - monomers[index - 1].x;
                auto const dy = y - monomers[index - 1].y;
                auto const dz = z - monomers[index - 1].z;
                auto const bond_sq = dx * dx + dy * dy + dz * dz;
                if (bond_sq < min_bond_sq || bond_sq > max_bond_sq) {
                    continue;
                }
            }

            if (index + 1 < n_monomers) {
                auto const dx = x - monomers[index + 1].x;
                auto const dy = y - monomers[index + 1].y;
                auto const dz = z - monomers[index + 1].z;
                auto const bond_sq = dx * dx + dy * dy + dz * dz;
                if (bond_sq < min_bond_sq || bond_sq > max_bond_sq) {
                    continue;
                }
            }

            // Displace
            release(monomer.x, monomer.y, monomer.z);

            if (occupy(index, x, y, z)) {
                monomer.x = x;
                monomer.y = y;
                monomer.z = z;
                accepts++;
            } else {
                occupy(index, monomer.x, monomer.y, monomer.z);
            }
        }
    }

    std::clog << double(accepts) / 200 / 100000 << '\n';
}
