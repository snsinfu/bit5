#pragma once

#include "misc/buffer.h"
#include "misc/matrix.h"


class Sandpile
{
    using State = int;

    struct PointChange {
        int   x;
        int   y;
        State delta;
    };

public:
    Sandpile(int width, int height);

    int        width() const;
    int        height() const;
    int const& operator()(int x, int y) const;
    int&       operator()(int x, int y);
    int        tick();

private:
    int updateChangeset();

private:
    matrix<State>       _vertices;
    buffer<PointChange> _changeset;
};


inline int
Sandpile::width() const
{
    return _vertices.cols();
}

inline int
Sandpile::height() const
{
    return _vertices.rows();
}

inline int const&
Sandpile::operator()(int x, int y) const
{
    return _vertices(y, x);
}

inline int&
Sandpile::operator()(int x, int y)
{
    return _vertices(y, x);
}


void randomize(Sandpile& sandpile);
void initialize(Sandpile& sandpile, int state);
