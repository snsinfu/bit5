#pragma once


template<typename T>
class maybe_index
{
public:
    maybe_index() = default;

    maybe_index(T index)
        : _code{increment(index)}
    {
    }

    explicit operator bool() const
    {
        return _code != 0;
    }

    bool operator==(T index) const
    {
        return _code == increment(index);
    }

    bool operator!=(T index) const
    {
        return _code != increment(index);
    }

    T value() const
    {
        return decrement(_code);
    }

    maybe_index& operator=(T value)
    {
        _code = increment(value);
        return *this;
    }

    void reset()
    {
        _code = 0;
    }

private:
    static T increment(T value)
    {
        return ++value;
    }

    static T decrement(T value)
    {
        return --value;
    }

private:
    T _code = 0;
};
