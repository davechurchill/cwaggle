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

class EntityController_OrbitalConstruction : public EntityController
{
    std::shared_ptr<World> m_world;
    Entity          m_robot;
    SensorReading   m_reading;
    double          m_threshold[2] = { 0.65, 0.8 };

public:

    EntityController_OrbitalConstruction(Entity robot, std::shared_ptr<World> world)
        : m_robot(robot)
        , m_world(world)
    {

    }

    virtual EntityAction getAction()
    {
        // read the sensors and store it in m_reading
        SensorTools::ReadSensorArray(m_robot, m_world, m_reading);

        const double MaxAngularSpeed = 0.3;
        const double ForwardSpeed = 2;

        if (m_reading.leftObstacle > 0)
        {
            m_previousAction = EntityAction(ForwardSpeed, MaxAngularSpeed);
            return m_previousAction;
        }

        size_t type = m_robot.getComponent<CRobotType>().type;
        bool innie = type == 1;

        if (m_reading.rightNest >= m_reading.midNest && m_reading.midNest >= m_reading.leftNest)
        {
            // The gradient is in the desired orientation with the highest
            // sensed value to the right, then the centre value in the middle,
            // followed by the lowest on the left.

            // These conditions steer in (for an innie) and out (for an outie)
            // to nudge a puck inwards or outwards.
            if (innie && m_reading.rightPucks > 0)
            {
                m_previousAction = EntityAction(ForwardSpeed, MaxAngularSpeed);
                return m_previousAction;
            }
            else if (!innie && m_reading.leftPucks > 0)
            {
                m_previousAction = EntityAction(ForwardSpeed, -MaxAngularSpeed);
                return m_previousAction;
            }

            // We now act to maintain the centre value at the desired isoline.
            if (m_reading.midNest < m_threshold[type])
            {
                m_previousAction = EntityAction(ForwardSpeed, 0.3 * MaxAngularSpeed);
                return m_previousAction;
            }
            else 
            {
                m_previousAction = EntityAction(ForwardSpeed, -0.3 * MaxAngularSpeed);
                return m_previousAction;
            }
        }
        else if (m_reading.midNest >= m_reading.rightNest && m_reading.midNest >= m_reading.leftNest)
        {
            // We are heading uphill of the gradient, turn left to return to a
            // clockwise orbit.
            m_previousAction = EntityAction(ForwardSpeed, -MaxAngularSpeed);
            return m_previousAction;
        }
        else
        {
            // We are heading downhill, turn right to return to clockwise orbit.
            m_previousAction = EntityAction(ForwardSpeed, MaxAngularSpeed);
            return m_previousAction;
        }
    }
};

struct OrbitalConstructionConfig
{
    double maxAngularSpeed = 0.3;
    double forwardSpeed = 2;
    double thresholds[2] = { 0.7, 0.8 };
};

namespace EntityControllers
{
    EntityAction OrbitalConstruction(Entity e, 
                                     std::shared_ptr<World> world, 
                                     const SensorReading & reading,
                                     const OrbitalConstructionConfig & config)
    {
        if (reading.leftObstacle > 0)
        {
            return EntityAction(config.forwardSpeed, config.maxAngularSpeed);
        }

        size_t type = e.getComponent<CRobotType>().type;
        bool innie = type == 1;

        if (reading.rightNest >= reading.midNest && reading.midNest >= reading.leftNest)
        {
            // The gradient is in the desired orientation with the highest
            // sensed value to the right, then the centre value in the middle,
            // followed by the lowest on the left.

            // These conditions steer in (for an innie) and out (for an outie)
            // to nudge a puck inwards or outwards.
            if (innie && reading.rightPucks > 0)
            {
                return EntityAction(config.forwardSpeed, config.maxAngularSpeed);
            }
            else if (!innie && reading.leftPucks > 0)
            {
                return EntityAction(config.forwardSpeed, -config.maxAngularSpeed);
            }

            // We now act to maintain the centre value at the desired isoline.
            if (reading.midNest < config.thresholds[type])
            {
                return EntityAction(config.forwardSpeed, 0.3 * config.maxAngularSpeed);
            }
            else
            {
                return EntityAction(config.forwardSpeed, -0.3 * config.maxAngularSpeed);
            }
        }
        else if (reading.midNest >= reading.rightNest && reading.midNest >= reading.leftNest)
        {
            // We are heading uphill of the gradient, turn left to return to a
            // clockwise orbit.
            return EntityAction(config.forwardSpeed, -config.maxAngularSpeed);
        }
        else
        {
            // We are heading downhill, turn right to return to clockwise orbit.
            return EntityAction(config.forwardSpeed, config.maxAngularSpeed);
        }
    }
};