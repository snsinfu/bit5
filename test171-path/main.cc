#include <iostream>
#include <vector>


int enumerate_subpaths(
    std::vector<std::vector<int>> const& costs,
    std::size_t pos,
    int cost,
    unsigned trace = 0
)
{
    auto const num_nodes = costs.size();
    auto const trace_mask = (1u << num_nodes) - 1u;

    if (cost < 0) {
        return 0;
    }

    if (trace == trace_mask) {
        if (cost == 0) {
            return 1;
        } else {
            return 0;
        }
    }

    int paths = 0;

    for (std::size_t next = 0; next < num_nodes; next++) {
        if (trace & (1u << next)) {
            continue;
        }

        auto const next_cost = cost - costs[pos][next];
        auto const next_trace = trace | (1u << next);

        paths += enumerate_subpaths(costs, next, next_cost, next_trace);
    }

    return paths;
}

int enumerate_paths(
    std::vector<std::vector<int>> const& costs,
    std::size_t start,
    int cost
)
{
    return enumerate_subpaths(costs, start, cost - costs[0][start], 1u);
}


int main()
{
    // A complete, directed, weighted graph is given. We call the sum of weight
    // along a path the cost of the path. Find a circuit having given cost.

    std::vector<std::vector<int>> const costs = {
        {1, 2, 3, 4},
        {5, 6, 7, 1},
        {2, 3, 4, 5},
        {6, 7, 1, 2},
    };
    auto const cost = 10;
    auto const num_nodes = costs.size();

    int paths = 0;
    for (std::size_t start = 0; start < num_nodes; start++) {
        paths += enumerate_paths(costs, start, cost);
    }
    std::cout << paths << '\n';

    // ?
}
