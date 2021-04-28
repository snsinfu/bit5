#include <iostream>
#include <sstream>


int main()
{
    do {
        std::istringstream stream{"123 "};
        double value;
        stream >> value;

        if (!stream) {
            std::cout << "error\n";
            break;
        }

        if (stream.get() != std::istringstream::traits_type::eof()) {
            std::cout << "leftover\n";
            break;
        }

        std::cout << "value: " << value << '\n';
    } while(false);
}
