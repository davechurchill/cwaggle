#pragma once

#include "Vec2.hpp"

class LineBody
{
public:
    Vec2 s;
    Vec2 e;
    double r = 1.0;
    
    LineBody() {}

    LineBody(Vec2 start, Vec2 end, double radius)
        : s(start), e(end), r(radius) { }
}; 