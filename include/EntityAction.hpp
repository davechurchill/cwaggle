#pragma once

#include "Entity.hpp"
#include "Components.hpp"

#include <cassert>

class EntityAction
{
    double  m_angularSpeed = 0;
    double  m_speed        = 0;
    
public:
        
    EntityAction() {}
    EntityAction(double speed, double angle)
        : m_angularSpeed(angle), m_speed(speed) {}

    inline const auto speed()        const { return m_speed; }
    inline const auto angularSpeed() const { return m_angularSpeed; }

    virtual void doAction(Entity e, double timeStep)
    {
        if (!e.hasComponent<CSteer>())
        {
            e.addComponent<CSteer>();
        }

        e.getComponent<CSteer>().angle += m_angularSpeed * timeStep;
        e.getComponent<CSteer>().speed  = m_speed;
    }
};