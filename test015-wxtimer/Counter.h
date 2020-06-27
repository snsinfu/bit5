#pragma once

#include <random>


class Counter
{
public:
    int  Get() const;
    void Reset();
    void Reset(int count);
    void Tick();

private:
    int             _count = 0;
    std::mt19937_64 _random;
};
