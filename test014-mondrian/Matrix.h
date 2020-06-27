#pragma once

#include <cstddef>
#include <vector>


template<typename T>
class Matrix
{
public:
    Matrix(int rows, int columns)
        : _rows{rows}
        , _columns{columns}
        , _store(static_cast<std::size_t>(rows * columns))
    {
    }

    int rows() const
    {
        return _rows;
    }

    int columns() const
    {
        return _columns;
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
        return static_cast<std::size_t>(row + _rows * col);
    }

private:
    int _rows;
    int _columns;
    std::vector<T> _store;
};
