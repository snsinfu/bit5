#pragma once

#include <cstddef>
#include <cstdint>
#include <vector>


template<typename T>
class power_of_two
{
public:
    power_of_two(unsigned exp)
        : _exp{exp}
    {
    }

    unsigned exponent() const
    {
        return _exp;
    }

    T value() const
    {
        return T{1} << _exp;
    }

    T mask() const
    {
        return value() - 1;
    }

private:
    unsigned _exp = 0;
};


template<typename T>
class cubic_lattice
{
public:
    using index_type = std::int32_t;

    explicit cubic_lattice(power_of_two<index_type> size)
        : _size{size}
        , _store(std::size_t{1} << size.exponent() * 3)
    {
    }

    T& operator()(index_type x, index_type y, index_type z)
    {
        return _store[locate(x, y, z)];
    }

    T const& operator()(index_type x, index_type y, index_type z) const
    {
        return _store[locate(x, y, z)];
    }

private:
    std::size_t locate(index_type x, index_type y, index_type z) const
    {
        auto const exp = _size.exponent();
        auto const mask = _size.mask();
        return std::size_t(x & mask) +
            (std::size_t(y & mask) << exp) +
            (std::size_t(z & mask) << exp * 2);
    }

private:
    power_of_two<index_type> _size;
    std::vector<T>           _store;
};
