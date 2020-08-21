#include <algorithm>
#include <iostream>
#include <vector>


enum occupation {
    Empty, B, A, AA
};

static bool has_domain(std::vector<int> const& state, long minsize);


int main()
{
    std::vector<int> const init = {
        Empty, A, A, B, B, B
    };
    long const minsize = 3;

    std::vector<int> state = init;

    long count_all = 0;
    long count_domain = 0;

    do {
        count_all++;
        count_domain += has_domain(state, minsize);

        std::next_permutation(state.begin(), state.end());
    } while (state != init);

    std::cout
        << count_domain
        << '/'
        << count_all
        << '\t'
        << count_domain * 100.0 / count_all
        << "%\n";
}

static bool has_domain(std::vector<int> const& state, long minsize)
{
    // Detect (B Empty*){3}

    std::size_t i = 0;
    for (; i < state.size(); i++) {
        if (state[i] == B) {
            break;
        }
    }

    long cont = 0;
    for (; i < state.size(); i++) {
        switch (state[i]) {
        case B:
            cont++;
            continue;

        case Empty:
            continue;

        default:
            break;
        }
        break;
    }

    return cont >= minsize;
}
