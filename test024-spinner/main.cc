#include <chrono>
#include <iostream>
#include <string>
#include <thread>


int main()
{
    char spinner[4] = {'/', '-', '\\', '|'};

    for (int i = 0; i < 10000; i++) {
        std::cout
            << "  "
            << i
            << ' '
            << spinner[i % 4]
            << ' '
            << std::string(i, '=')
            << '\r'
            << std::flush;

        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
}
