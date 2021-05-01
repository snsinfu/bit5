#include <initializer_list>
#include <string>
#include <vector>


class header_assertion
{
    enum class check_mode
    {
        none,
        existence,
        content,
    };

public:
    header_assertion(bool yesno)
        : _check{yesno ? check_mode::existence : check_mode::none}
    {
    }

    header_assertion(std::initializer_list<std::string> names)
        : _check{check_mode::content}
        , _expected_names{names}
    {
    }

    bool check() const
    {
        switch (_check) {
        case check_mode::none:
            return true;

        case check_mode::existence:
            return true;

        case check_mode::content:
            return true;
        }
    }

private:
    check_mode _check;
    std::vector<std::string> _expected_names;
};

struct parse_options
{
    header_assertion header = true;
};

void parse(parse_options const& opts = {})
{
    opts.header.check();
}

int main()
{
    parse();
    parse({ .header = true });
    parse({ .header = false });
    parse({ .header = {"row", "column", "value"} });
}
