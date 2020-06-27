#pragma once

#include <cstddef>
#include <cstdlib>
#include <new>
#include <type_traits>


// std::vector is slow for my simulation. The bottleneck is value-initialization
// of elements. So, I use buffer<T> class defined here that does not initialize
// elements. The sandpile calculation gets 2x faster if I use this class instead
// of std::vector.

template<typename T>
class buffer
{
    static_assert(std::is_trivial_v<T>);

    static constexpr std::size_t linear_growth = 32;
    static constexpr std::size_t exponential_growth = 2;

public:
    buffer() = default;
    buffer(buffer&&) = delete;
    buffer(buffer const&) = delete;
    buffer& operator=(buffer&&) = delete;
    buffer& operator=(buffer const&) = delete;

    ~buffer() noexcept
    {
        std::free(_ptr);
    }

    void clear() noexcept
    {
        _size = 0;
    }

    void push_back(T const& value)
    {
        if (_size == _capacity) {
            realloc();
        }
        _ptr[_size++] = value;
    }

    T const* begin() const noexcept
    {
        return _ptr;
    }

    T const* end() const noexcept
    {
        return _ptr + _size;
    }

private:
    void realloc()
    {
        _capacity += linear_growth;
        _capacity *= exponential_growth;

        auto new_ptr = static_cast<T*>(std::realloc(_ptr, _capacity * sizeof(T)));
        if (!new_ptr) {
            throw std::bad_alloc();
        }
        _ptr = new_ptr;
    }

private:
    T*          _ptr      = nullptr;
    std::size_t _size     = 0;
    std::size_t _capacity = 0;
};
