#include "Counter.h"

#include <random>


int
Counter::Get() const
{
    return _count;
}

void
Counter::Reset()
{
    _count = 0;
}

void
Counter::Reset(int count)
{
    _count = count;
}

void
Counter::Tick()
{
    std::uniform_int_distribution<int> distr{0, 2};
    int delta = distr(_random);
    if (delta == 0) {
        delta = -1;
    }
    _count += delta;
}
