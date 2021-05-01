#include <algorithm>
#include <iostream>
#include <string_view>


std::string_view split_consume(std::string_view& str, char delim)
{
    auto const delim_pos = std::min(str.find(delim), str.size());
    auto const next_start = std::min(delim_pos + 1, str.size());
    auto const token = str.substr(0, delim_pos);
    str = str.substr(next_start);
    return token;
}

int main()
{
    std::string_view text{"aaa|bbb||"};
    if (!text.empty()) {
        std::string_view token;
        do {
            token = split_consume(text, '|');
            std::cout << "<" << token << ">\n";
        } while (token.end() != text.end());
    }
}
