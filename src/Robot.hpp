#pragma once

#include "CircleBody.hpp"
#include "RobotAction.hpp"

class Robot
{
    CircleBody *    m_body  = nullptr;
    size_t          m_id    = 0;

    RobotAction     m_previousAction;

public:

    Robot() {}

    Robot(CircleBody * body, size_t id)
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