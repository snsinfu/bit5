#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>


static uint32_t hash(long value)
{
    // MurmurHash2 mixing
    uint32_t h = (uint32_t) value;
    h ^= h >> 13;
    h *= 0x5bd1e995;
    h ^= h >> 15;
    return h;
}


int main(void)
{
    uint32_t n_buckets = 61;
    double *buckets = calloc(n_buckets, sizeof *buckets);

    for (long i = 0; i < 1000000; i++) {

        // https://lemire.me/blog/2016/06/27/a-fast-alternative-to-the-modulo-reduction/
        uint64_t index = hash(i);
        index *= n_buckets;
        index >>= 32;

        buckets[index] += 1;
    }

    for (size_t i = 0; i < n_buckets; i++) {
        printf("%zu\t%g\n", i, buckets[i]);
    }

    free(buckets);

    return 0;
}
