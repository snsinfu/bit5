#include <iostream>
#include <string_view>

class splitter
{
public:
    explicit splitter(std::string_view text)
        : _text{text}, _prev_end{text.begin()}
    {
    }

    bool is_empty() const
    {
        return _prev_end == _text.end();
    }

    std::string_view split(char delim)
    {
        auto const delim_pos = _text.find(delim);
        auto token_end = delim_pos;
        auto next_start = delim_pos + 1;
        if (delim_pos == std::string_view::npos) {
            token_end = _text.size();
            next_start = _text.size();
        }

        auto const token = _text.substr(0, token_end);
        _text = _text.substr(next_start);
        _prev_end = token.end();
        return token;
    }

private:
    std::string_view _text;
    std::string_view::iterator _prev_end;
};

int main()
{
    splitter toks{"aaa|bbb||"};

    while (!toks.is_empty()) {
        std::cout << "<" << toks.split('|') << ">\n";
    }
}
