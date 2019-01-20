#pragma once

#include "CircleBody.hpp"

class Puck
{
    CircleBody *       m_body    = nullptr;
    size_t m_id        = 0;
    bool   m_flashing  = false;

public:

    Puck() {}

    Puck(CircleBody * body, size_t id)
        : m_body(body)
        , m_id(id)
    {

    }

    inline size_t id() const
    {
        return m_id;
    }

    inline CircleBody * body() const
    {
        return m_body;
    }
};