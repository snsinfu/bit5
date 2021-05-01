#include <iostream>
#include <string_view>

class splitter
{
public:
    explicit splitter(std::string_view text)
        : _start{text.data()}
        , _text_end{text.data() + text.size()}
        , _prev_end{text.data()}
    {
    }

    bool is_empty() const
    {
        return _prev_end == _text_end;
    }

    std::string_view split(char delim)
    {
        char const* end = _start;
        while (end != _text_end && *end != delim) {
            ++end;
        }
        std::string_view const token{_start, std::size_t(end - _start)};
        _prev_end = end;
        _start = (end == _text_end) ? end : end + 1;
        return token;
    }

private:
    char const* _start;
    char const* _text_end;
    char const* _prev_end;
};

int main()
{
    splitter toks{"aaa|bbb||"};

    while (!toks.is_empty()) {
        std::cout << "<" << toks.split('|') << ">\n";
    }
}
