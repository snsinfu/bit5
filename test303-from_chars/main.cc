#include <charconv>
#include <iostream>
#include <string>


int main()
{
    // As of now (clang 12 / gcc 10), both libc++ and libstdc++ misses the
    // floating-point overload of std::from_chars...
    //
    // g++ with libstdc++
    //   error: no matching function for call to 'from_chars(const char* const&, const char* const&, double&)'
    //
    // clang++ with libc++
    //   error: call to deleted function 'from_chars'

    std::string const text = "xxx";

    double value;
    auto const begin = text.data();
    auto const end = text.data() + text.size();
    auto const [remain, err] = std::from_chars(begin, end, value);

    std::cout << remain << '\n';
}
