#pragma once

#include <sstream>
#include <string>

template<typename... Ts>
std::string format(Ts const&... values)
{
    std::ostringstream out;
    (out << ... << values);
    return out.str();
}
