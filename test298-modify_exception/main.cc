#include <cstddef>
#include <iostream>
#include <stdexcept>

class parse_error : public std::runtime_error
{
public:
    using std::runtime_error::runtime_error;
    std::size_t line_number = 0;
};

bool do_parse(std::size_t line_number)
{
    if (line_number == 5) {
        throw parse_error{"syntax error"};
    }
    return true;
}

void parse()
{
    std::size_t line_number = 1;
    try {
        while (do_parse(line_number)) {
            line_number++;
        }
    } catch (parse_error& err) {
        // Modify and rethrow
        err.line_number = line_number;
        throw;
    }
}

int main()
{
    try {
        parse();
    } catch (parse_error const& err) {
        std::cerr
            << "error: "
            << err.what()
            << " (at line "
            << err.line_number
            << ")\n";
    }
}
