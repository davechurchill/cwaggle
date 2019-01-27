#pragma once

#include "Vec2.hpp"
#include "Sensors.h"
#include "World.hpp"
#include "Entity.hpp"
#include "Components.hpp"

Sensor::Sensor() {}
Sensor::Sensor(size_t ownerID, double angle, double distance)
    : m_ownerID(ownerID), m_angle(angle*3.1415926 / 180.0), m_distance(distance) { }

Vec2 Sensor::getPosition()
{
    const Vec2 & pos = Entity(m_ownerID).getComponent<CTransform>().p;
    double sumAngle = m_angle + Entity(m_ownerID).getComponent<CSteer>().angle;
    return pos + Vec2(m_distance * cos(sumAngle), m_distance * sin(sumAngle));
}

double Sensor::angle() const
{
    return m_angle;
}

double Sensor::distance() const
{
    return m_distance;
}

GridSensor::GridSensor(size_t ownerID, double angle, double distance)
    : Sensor(ownerID, angle, distance) {}

double GridSensor::getReading(World & world)
{
    if (world.getGrid().width() == 0) { return 0; }
    Vec2 sPos = getPosition();
    size_t gX = (size_t)round(sPos.x / world.width());
    size_t gY = (size_t)round(sPos.y / world.height());
    return world.getGrid().get(gX, gY);
}

PuckSensor::PuckSensor(size_t ownerID, double angle, double distance, double radius)
    : Sensor(ownerID, angle, distance)
{
    m_radius = radius;
}

double PuckSensor::getReading(World & world)
{
    Vec2 pos = getPosition();
    for (auto puck : world.getEntities("puck"))
    {
        auto & t = puck.getComponent<CTransform>();
        auto & b = puck.getComponent<CCircleBody>();

        // collision with a puck
        if (t.p.distSq(pos) < (m_radius + b.r)*(m_radius + b.r))
        {
            return 1.0;
        }
    }
    return 0;
}

double PuckSensor::radius() const
{
    return m_radius;
}


ObstacleSensor::ObstacleSensor(size_t ownerID, double angle, double distance, double radius)
    : Sensor(ownerID, angle, distance)
{
    m_radius = radius;
}

double ObstacleSensor::getReading(World & world)
{
    Vec2 pos = getPosition();
    for (auto e : world.getEntities())
    {
        if (!e.hasComponent<CCircleBody>()) { continue; }
        if (m_ownerID == e.id()) { continue; }

        auto & t = e.getComponent<CTransform>();
        auto & b = e.getComponent<CCircleBody>();

        // collision with a puck
        if (t.p.distSq(pos) < (m_radius + b.r)*(m_radius + b.r))
        {
            return 1.0;
        }
    }
    return 0;
}

double ObstacleSensor::radius() const
{
    return m_radius;
}