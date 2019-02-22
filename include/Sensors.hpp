#pragma once

#include <memory>

#include "World.hpp"
#include "Entity.hpp"
#include "Components.hpp"

class Sensor
{
protected:

    size_t m_ownerID;         // entity that owns this sensor
    double m_angle = 0;     // angle sensor is placed w.r.t. owner heading
    double m_distance = 0;  // distance from center of owner

public:

    Sensor() {}
    Sensor(size_t ownerID, double angle, double distance)
        : m_ownerID(ownerID), m_angle(angle*3.1415926 / 180.0), m_distance(distance) { }

    inline virtual Vec2 getPosition()
    {
        const Vec2 & pos = Entity(m_ownerID).getComponent<CTransform>().p;
        double sumAngle = m_angle + Entity(m_ownerID).getComponent<CSteer>().angle;
        return pos + Vec2(m_distance * cos(sumAngle), m_distance * sin(sumAngle));
    }

    inline virtual double angle() const
    {
        return m_angle;
    }

    inline virtual double distance() const
    {
        return m_distance;
    }

    virtual double getReading(std::shared_ptr<World> world) = 0;
};


class GridSensor : public Sensor
{
    
public:

    GridSensor(size_t ownerID, double angle, double distance)
        : Sensor(ownerID, angle, distance) {}

    inline virtual double getReading(std::shared_ptr<World> world)
    {
        if (world->getGrid().width() == 0) { return 0; }
        Vec2 sPos = getPosition();
        size_t gX = (size_t)round(world->getGrid().width()  * sPos.x / world->width());
        size_t gY = (size_t)round(world->getGrid().height() * sPos.y / world->height());
        return world->getGrid().get(gX, gY);
    }
};


class PuckSensor : public Sensor
{
    double m_radius;

public:

    PuckSensor(size_t ownerID, double angle, double distance, double radius)
        : Sensor(ownerID, angle, distance)
    {
        m_radius = radius;
    }

    inline double getReading(std::shared_ptr<World> world)
    {
        double sum = 0;
        Vec2 pos = getPosition();
        for (auto puck : world->getEntities("puck"))
        {
            auto & t = puck.getComponent<CTransform>();
            auto & b = puck.getComponent<CCircleBody>();

            // collision with a puck
            if (t.p.distSq(pos) < (m_radius + b.r)*(m_radius + b.r))
            {
                sum += 1.0;
            }
        }
        return sum;
    }

    inline double radius() const
    {
        return m_radius;
    }

};

class ObstacleSensor : public Sensor
{
    double m_radius;

public:

    ObstacleSensor(size_t ownerID, double angle, double distance, double radius)
        : Sensor(ownerID, angle, distance)
    {
        m_radius = radius;
    }

    inline double getReading(std::shared_ptr<World> world)
    {
        double sum = 0;
        Vec2 pos = getPosition();
        for (auto e : world->getEntities())
        {
            if (!e.hasComponent<CCircleBody>()) { continue; }
            if (m_ownerID == e.id()) { continue; }

            auto & t = e.getComponent<CTransform>();
            auto & b = e.getComponent<CCircleBody>();

            // collision with a puck
            if (t.p.distSq(pos) < (m_radius + b.r)*(m_radius + b.r))
            {
                sum += 1.0;
            }
        }
        return sum;
    }

    inline double radius() const
    {
        return m_radius;
    }
};