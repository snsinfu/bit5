#pragma once

#include <iostream>
#include <stdexcept>


#define TRACE_ASSERT(pred) do_trace_assert(bool(pred), #pred, __FILE__, __LINE__)


inline void do_trace_assert(
    bool        cond,
    char const* expr,
    char const* file,
    unsigned    line
)
{
    static char const* const ansi_green = "\x1b[32m";
    static char const* const ansi_red = "\x1b[31m";
    static char const* const ansi_dim = "\x1b[90m";
    static char const* const ansi_reset = "\x1b[m";

    if (cond) {
        std::clog << ansi_green << "PASS" << ansi_reset;
    } else {
        std::clog << ansi_red << "FAIL" << ansi_reset;
    }

    std::clog
        << ansi_dim
        << " ("
        << file
        << ':'
        << line
        << ") "
        << ansi_reset
        << expr
        << '\n';

    if (!cond) {
        throw std::logic_error{"assertion failed"};
    }
}
