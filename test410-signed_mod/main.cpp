#include <cstdlib>
#include <iostream>


int main()
{
    int const index = -5;
    int const mod = 3;

    // -2 (wrong)
    std::cout << index % mod << '\n';

    // 2 (right)
    std::cout << (mod - index % mod) % mod << '\n';

    // 2 (right)
    std::cout << std::abs(index % mod) << '\n';
}
