#pragma once

#include "Simulator.hpp"
#include "Entity.hpp"
#include "EntityAction.hpp"
#include "SensorTools.hpp"

class EntityController
{
protected:

    EntityAction    m_previousAction;

public:

    EntityController() {}
    virtual EntityAction getAction() = 0;
};

class EntityController_Turn : public EntityController
{
    double          m_angularSpeed;
    double          m_speed;

public:

    EntityController_Turn(double angularSpeed, double speed)
        : m_angularSpeed(angularSpeed)
        , m_speed(speed) 
    { 
        
    }

    virtual EntityAction getAction()
    {
        m_previousAction = EntityAction(m_angularSpeed, m_speed);
        return m_previousAction;
    }
};
