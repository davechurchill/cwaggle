#pragma once

#include "Simulator.hpp"
#include "Entity.hpp"
#include "EntityAction.hpp"


class EntityController
{

public:

    EntityController() {}
    virtual EntityAction getAction() = 0;
};

class EntityController_Turn : public EntityController
{
    double          m_angularSpeed;
    double          m_speed;
    EntityAction    m_previousAction;

public:

    EntityController_Turn(double angularSpeed, double speed)
        : m_angularSpeed(angularSpeed)
        , m_speed(speed) 
        , m_previousAction(angularSpeed, speed)
    { 
        
    }

    virtual EntityAction getAction()
    {
        return m_previousAction;
    }
};