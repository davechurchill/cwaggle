#pragma once

#include <math.h>

class Vec2
{
public:

    double x = 0;
    double y = 0;

    Vec2() { } 
    Vec2(double xIn, double yIn) : x(xIn), y(yIn) { }

    inline Vec2 operator + (const Vec2 & rhs) const
    {
        return Vec2(x + rhs.x, y + rhs.y);
    }

    inline Vec2 operator - (const Vec2 & rhs) const
    {
        return Vec2(x - rhs.x, y - rhs.y);
    }

    inline Vec2 operator / (double val) const
    {
        return Vec2(x / val, y / val);
    }

    inline Vec2 operator * (double val) const
    {
        return Vec2(x * val, y * val);
    }

    inline bool operator == (const Vec2 & rhs) const
    {
        return x == rhs.x && y == rhs.y;
    }

    inline bool operator != (const Vec2 & rhs) const
    {
        return !(*this == rhs);
    }

    inline void operator += (const Vec2 & rhs)
    {
        x += rhs.x;
        y += rhs.y;
    }

    inline void operator -= (const Vec2 & rhs)
    {
        x -= rhs.x;
        y -= rhs.y;
    }

    inline void operator *= (double val)
    {
        x *= val;
        y *= val;
    }

    inline void operator /= (double val)
    {
        x /= val;
        y /= val;
    }

    inline double dist(const Vec2 & rhs) const
    {
        return sqrt(distSq(rhs));
    }

    inline double distSq(const Vec2 & rhs) const
    {
        return (x - rhs.x)*(x - rhs.x) + (y - rhs.y)*(y - rhs.y);
    }

    inline double length() const
    {
        return sqrt(x*x + y*y);
    }

    inline Vec2 normalize() const
    {
        double l = length();
        return Vec2(x / l, y / l);
    }
};