#pragma once

#include <memory>

class World;


class Sensor
{
protected:

    size_t m_ownerID;         // entity that owns this sensor
    double m_angle = 0;     // angle sensor is placed w.r.t. owner heading
    double m_distance = 0;  // distance from center of owner

public:

    Sensor();
    Sensor(size_t ownerID, double angle, double distance);

    Vec2 getPosition();
    virtual double angle() const;
    virtual double distance() const;
    virtual double getReading(std::shared_ptr<World> world) = 0;
};


class GridSensor : public Sensor
{
    
public:

    GridSensor(size_t ownerID, double angle, double distance);
    virtual double getReading(std::shared_ptr<World> world);
};


class PuckSensor : public Sensor
{
    double m_radius;

public:

    PuckSensor(size_t ownerID, double angle, double distance, double radius);
    virtual double getReading(std::shared_ptr<World> world);
    double radius() const;
};

class ObstacleSensor : public Sensor
{
    double m_radius;

public:

    ObstacleSensor(size_t ownerID, double angle, double distance, double radius);
    virtual double getReading(std::shared_ptr<World> world);
    double radius() const;
};