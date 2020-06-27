#pragma once

#include <cstddef>
#include <vector>


template<typename T>
class matrix
{
public:
    matrix(int rows, int cols)
        : _rows{rows}
        , _cols{cols}
        , _store(static_cast<std::size_t>(rows * cols))
    {
    }

    int rows() const
    {
        return _rows;
    }

    int cols() const
    {
        return _cols;
    }

    T const& operator()(int row, int col) const
    {
        return _store[locate(row, col)];
    }

    T& operator()(int row, int col)
    {
        return _store[locate(row, col)];
    }

private:
    std::size_t locate(int row, int col) const
    {
        return static_cast<std::size_t>(row * _cols + col);
    }

private:
    int _rows;
    int _cols;
    std::vector<T> _store;
};
