#include <inttypes.h>
#include <stdio.h>


typedef uint64_t romu_state;

void     romu_seed(romu_state *state, uint32_t seed);
uint32_t romu_next(romu_state *state);


int main(void)
{
    romu_state state;

    romu_seed(&state, 1);
    romu_next(&state);

    for (int i = 0; i < 100; i++) {
        uint32_t v = romu_next(&state);
        printf("%" PRIu32 "\n", v);
    }
}


static inline uint64_t rotl(uint64_t x, int n)
{
    return (x << n) | (x >> (64 - n));
}

void romu_seed(romu_state *state, uint32_t seed)
{
    *state = ((uint64_t) seed << 32) | 1u;
}

uint32_t romu_next(romu_state *state)
{
    uint32_t result = (uint32_t) *state;
    *state = rotl(*state, 32) * 15241094284759029579u;
    return result;
}
