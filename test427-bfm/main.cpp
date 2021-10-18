// Bond Fluctuation Model
// https://en.wikipedia.org/wiki/Bond_Fluctuation_Model

#include <cmath>
#include <cstddef>
#include <cstdint>
#include <iostream>
#include <random>
#include <vector>


struct lattice_state
{
    std::uint16_t monomer = 0;
};


struct lattice_index
{
    std::int32_t x = 0;
    std::int32_t y = 0;
    std::int32_t z = 0;
};


struct montecarlo_move
{
    std::uint16_t monomer   = 0;
    std::uint16_t direction = 0;
};


int main()
{
    // N-monomer polymer on an L-by-L-by-L cubec lattice.
    std::size_t const N = 256;
    std::size_t const L = 64;

    std::mt19937_64 random;
    std::vector<lattice_state> lattice(L * L * L);
    std::vector<lattice_index> monomers(N);

    auto locate = [=](lattice_index const& pos) {
        auto const x = std::size_t(pos.x);
        auto const y = std::size_t(pos.y);
        auto const z = std::size_t(pos.z);
        return x + (y + z * L) * L;
    };

    // Initialization
    lattice_index pos;

    std::int32_t const min_bond_sq = 4;
    std::int32_t const max_bond_sq = 10;

    for (std::size_t i = 0; i < N; i++) {
        monomers[i] = pos;
        lattice[locate(pos)].monomer = std::uint16_t(i) + 1;

        for (;;) {
            std::uniform_int_distribution<std::int32_t> delta{-3, 3};

            lattice_index bond;
            bond.x = delta(random);
            bond.y = delta(random);
            bond.z = delta(random);

            auto const bond_sq = bond.x * bond.x + bond.y * bond.y + bond.z * bond.z;
            if (bond_sq < min_bond_sq || bond_sq > max_bond_sq) {
                continue;
            }

            pos.x += bond.x;
            pos.y += bond.y;
            pos.z += bond.z;

            pos.x = (pos.x + L) & (L - 1);
            pos.y = (pos.y + L) & (L - 1);
            pos.z = (pos.z + L) & (L - 1);

            // FIXME: Should check all vertices of the cell.
            if (lattice[locate(pos)].monomer) {
                continue;
            }

            break;
        }
    }

    // MC simulation

    std::int32_t const sampling_steps = 100;

    std::vector<std::uint32_t> montecarlo_bits(N);

    for (std::int32_t step = 0; step < sampling_steps; step++) {

        // Batch-compute Monte-Carlo moves.
        for (std::size_t i = 0; i < N; ) {
            auto const bits = random();
            montecarlo_bits[i++] = std::uint32_t(bits);
            montecarlo_bits[i++] = std::uint32_t(bits >> 32);
        }

        lattice_index const steps[] = {
            {1, 0, 0}, {0, 1, 0}, {0, 0, 1},
            {-1, 0, 0}, {0, -1, 0}, {0, 0, -1},
        };

        for (auto const& mc : montecarlo_bits) {

            auto decode = [](std::uint32_t bits) -> montecarlo_move {
                auto const upper = bits >> 16;
                auto const lower = bits & 0xFFFF;
                return {
                    .monomer   = std::uint16_t(upper * N >> 16),
                    .direction = std::uint16_t(lower * 6 >> 16),
                };
            };

            auto const move = decode(mc);
            auto const step = steps[move.direction];
            auto const prev = monomers[move.monomer];

            auto pos = prev;

            pos.x += step.x;
            pos.y += step.y;
            pos.z += step.z;

            pos.x = (pos.x + L) & (L - 1);
            pos.y = (pos.y + L) & (L - 1);
            pos.z = (pos.z + L) & (L - 1);

            // FIXME: Should check all vertices of the cell.
            if (lattice[locate(pos)].monomer) {
                continue;
            }

            if (move.monomer > 0) {
                auto const left = monomers[move.monomer - 1];
                auto const dx = pos.x - left.x;
                auto const dy = pos.y - left.y;
                auto const dz = pos.z - left.z;
                auto const bond_sq = dx * dx + dy * dy + dz * dz;

                if (bond_sq < min_bond_sq || bond_sq > max_bond_sq) {
                    continue;
                }
            }

            if (move.monomer + 1 < N) {
                auto const right = monomers[move.monomer + 1];
                auto const dx = pos.x - right.x;
                auto const dy = pos.y - right.y;
                auto const dz = pos.z - right.z;
                auto const bond_sq = dx * dx + dy * dy + dz * dz;

                if (bond_sq < min_bond_sq || bond_sq > max_bond_sq) {
                    continue;
                }
            }

            monomers[move.monomer] = pos;

            // FIXME: Update lattice.
        }

        for (auto const& monomer : monomers) {
            std::cout << monomer.x << ' ' << monomer.y << ' ' << monomer.z << '\n';
        }
    }
}
