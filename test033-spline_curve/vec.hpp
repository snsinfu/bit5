// Basic three-dimensional vector library
#pragma once

#include <cmath>


struct vec
{
    double x = 0;
    double y = 0;
    double z = 0;
};


double dot(vec const& v1, vec const& v2);
double squared_norm(vec const& v);
double norm(vec const& v);
vec cross(vec const& v1, vec const& v2);
vec normalize(vec const& v);
vec operator+(vec const& v1, vec const& v2);
vec operator-(vec const& v1, vec const& v2);
vec operator*(double a, vec const& v);


inline double dot(vec const& v1, vec const& v2)
{
    return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
}


inline double squared_norm(vec const& v)
{
    return dot(v, v);
}


inline double norm(vec const& v)
{
    return std::sqrt(squared_norm(v));
}


inline vec cross(vec const& v1, vec const& v2)
{
    return vec {
        v1.y * v2.z - v2.y * v1.z,
        v1.z * v2.x - v2.z * v1.x,
        v1.x * v2.y - v2.x * v1.y,
    };
}


inline vec normalize(vec const& v)
{
    return 1 / norm(v) * v;
}


inline vec operator+(vec const& v1, vec const& v2)
{
    return vec{v1.x + v2.x, v1.y + v2.y, v1.z + v2.z};
}


inline vec operator-(vec const& v1, vec const& v2)
{
    return vec{v1.x - v2.x, v1.y - v2.y, v1.z - v2.z};
}


inline vec operator*(double a, vec const& v)
{
    return vec{a * v.x, a * v.y, a * v.z};
}
