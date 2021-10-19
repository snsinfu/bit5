#include <cstdint>
#include <iostream>


std::int32_t reme(std::int32_t x, std::int32_t L);


int main()
{
    std::int32_t const L = 16;
    std::int32_t x = -105;

    // Super naive Euclidean remainder.
    std::int32_t ax = x;
    while (ax < 0) {
        ax += L;
    }
    ax %= L;

    // Single divmod algorithm.
    std::int32_t bx = reme(x, L);

    // When L is a power of two and the signed integer representation is two's
    // complement, a single bit masking can handle both negative and positive
    // numbers correctly.
    std::int32_t cx = x & (L - 1);

    std::cout << "ax = " << ax << '\n';
    std::cout << "bx = " << bx << '\n';
    std::cout << "cx = " << cx << '\n';
}


// https://stackoverflow.com/a/66057959
// https://www.microsoft.com/en-us/research/publication/division-and-modulus-for-computer-scientists/
std::int32_t reme(std::int32_t numer, std::int32_t denom)
{
    auto r = numer % denom;
    if (r < 0) {
        if (denom > 0)
            r = r + denom;
        else
            r = r - denom;
    }
    return r;
}
