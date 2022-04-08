#include <cstddef>
#include <cstdint>
#include <iostream>


std::size_t hash_combine(std::size_t x, std::size_t y)
{
    constexpr std::size_t a = 0x9e3779b97f4a7c15; // 2^64 / (golden ratio)
    constexpr std::size_t b = 0xf1de83e19937733d; // Modular inverse of a
    x *= a;
    y *= b;
    return (x + b) ^ (y + a);
}


int main()
{
    std::cout << hash_combine(1, 0) << '\n';
    std::cout << hash_combine(0, 0) << '\n';
    std::cout << hash_combine(0, 1) << '\n';
    std::cout << hash_combine(0, 2) << '\n';
    std::cout << hash_combine(0, 3) << '\n';
    std::cout << hash_combine(0, 4) << '\n';
    std::cout << hash_combine(1, 2) << '\n';
}
