#include <stdio.h>


int
enumerate(int rank, unsigned state)
{
    const unsigned complete = (1u << rank) - 1;

    if (state == complete) {
        return 1;
    }

    int sum = 0;

    for (int i = 0; i < rank; i++) {
        const unsigned mask = 1u << i;

        if (state & mask) {
            continue;
        }

        state ^= mask;
        sum += enumerate(rank, state);
        state ^= mask;
    }

    return sum;
}


int
main(void)
{
    printf("%d\n", enumerate(5, 0));
}
