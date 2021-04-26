#include <cstddef>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <string>
#include <string_view>


class io_error : public std::runtime_error
{
public:
    using std::runtime_error::runtime_error;
};

class line_reader
{
public:
    explicit line_reader(std::istream& input)
        : _input{input}
    {
    }

    bool read()
    {
        if (!std::getline(_input, _line)) {
            if (!_input.eof()) {
                throw io_error{"error reading line"};
            }
            return false;
        }
        _line_number++;
        return true;
    }

    std::string_view line() const noexcept
    {
        return _line;
    }

    std::size_t line_number() const noexcept
    {
        return _line_number;
    }

private:
    std::istream& _input;
    std::string _line;
    std::size_t _line_number = 0;
};

int main()
{
    std::istringstream input{
        "# network config\n"
        "address 0.0.0.0\n"
        "port 3000\n"
        "\n"
        "# file paths\n"
        "datadir ./data\n"
        "logdir ./logs\n"
        "#"
    };
    line_reader lines{input};

    [&] {
        while (lines.read()) {

            // Skip comment and empty lines. Ugly. Design issues?
            while (lines.line().empty() || lines.line().front() == '#') {
                if (!lines.read()) {
                    return;
                }
            }

            std::cout
                << lines.line_number()
                << " | "
                << lines.line()
                << '\n';
        }
    }();
}
