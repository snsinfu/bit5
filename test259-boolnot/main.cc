#include <cassert>

int main() {
    // warning: bitwise negation of a boolean expression always evaluates to 'true'
    bool a = true;
    bool b = ~a;

    // In the expression ~a, first a is promoted to an int of value 1. Then,
    // ~a results to 0b1111...1110, which in turn is converted to bool as true
    // since the integral value is nonzero.
    //
    // Thus, the bitwise negation of true results in true.

    assert(b == true);
}
