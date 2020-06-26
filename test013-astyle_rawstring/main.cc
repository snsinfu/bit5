#include <iostream>


char const program_usage[] = R"(
usage: main [options]

options:
  -h  Show this help message and exit
)"
; // Dont't remove. This line makes AStyle happy)";


// The brace will be formatted.
int main() {
    std::cout << program_usage << '\n';
}
