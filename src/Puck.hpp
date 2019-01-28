#pragma once

#include "CircleBody.hpp"

class Puck
{
    size_t m_bodyID    = 0;
    size_t m_id        = 0;
    bool   m_flashing  = false;
    std::array<uint8_t, 4> m_color = { 0, 0, 255, 255 };

public:

    Puck() {}

    Puck(size_t id, size_t bodyID) 
        : m_id(id) , m_bodyID(bodyID) { }

    Puck(size_t id, size_t bodyID, decltype(m_color) color)
        : m_id(id), m_bodyID(bodyID), m_color(color) { }

    inline const auto id()      const { return m_id; }
    inline const auto bodyID()  const { return m_bodyID; }
    inline const auto color()   const { return m_color; }

    inline void setColor(decltype(m_color) color)
    {
        m_color = color;
    }

};