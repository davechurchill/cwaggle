#pragma once

#include "Vec2.hpp"

class CircleBody
{
public:
    Vec2    p       = { 0, 0 };
    Vec2    v       = { 0, 0 };
    Vec2    a       = { 0, 0 };
    double  m       = 0;
    double  r       = 0;
    size_t  id      = 0;
    bool    moved   = false;
    bool    collided = false;
    
    CircleBody() {}

    CircleBody(Vec2 position, double radius, int identifier)
        : p(position)
        , r(radius)
        , id(identifier)
        , m(radius * 10)
    {

    }
};