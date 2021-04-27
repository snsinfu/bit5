#include <cstddef>
#include <iostream>
#include <optional>
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

    std::optional<std::string_view> consume()
    {
        if (!ensure()) {
            return std::nullopt;
        }
        _consumed = true;
        return _line;
    }

    std::optional<std::string_view> peek()
    {
        if (!ensure()) {
            return std::nullopt;
        }
        return _line;
    }

    std::size_t line_number() const noexcept
    {
        return _line_number;
    }

private:
    bool ensure()
    {
        if (_consumed) {
            if (!read()) {
                return false;
            }
            _consumed = false;
        }
        return true;
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

private:
    std::istream& _input;
    std::string _line;
    std::size_t _line_number = 0;
    bool _consumed = true;
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

    for (;;) {
        // Skip comment and empty lines.
        for (;;) {
            std::string_view line;

            if (auto maybe_line = lines.peek()) {
                line = *maybe_line;
            } else {
                break;
            }

            if (line.empty() || line.front() == '#') {
                lines.consume();
                continue;
            }
            break;
        }

        // Print non-comment line.
        std::string_view line;

        if (auto maybe_line = lines.consume()) {
            line = *maybe_line;
        } else {
            break;
        }

        std::cout
            << lines.line_number()
            << " | "
            << line
            << '\n';
    }
}
