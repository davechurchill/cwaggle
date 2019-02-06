#pragma once

#include <sstream>

#include "Entity.hpp"
#include "World.hpp"
#include "Sensors.hpp"

struct SensorReading
{
    double leftNest = 0;
    double midNest = 0;
    double rightNest = 0;
    double leftPucks = 0;
    double rightPucks = 0;
    double leftObstacle = 0;
    double rightObstacle = 0;

    std::string toString()
    {
        std::stringstream ss;
        ss << "lNest: " << leftNest << "\n";
        ss << "mNest: " << midNest << "\n";
        ss << "rNest: " << rightNest << "\n";
        ss << "lPuck: " << leftPucks << "\n";
        ss << "rPuck: " << rightPucks << "\n";
        ss << "lObst: " << leftObstacle << "\n";
        ss << "rObst: " << rightObstacle;
        return ss.str();
    }
};

namespace SensorTools
{
    inline void ReadSensorArray(Entity e, std::shared_ptr<World> world, SensorReading & reading)
    {
        reading = {};

        auto & sensors = e.getComponent<CSensorArray>();
        for (auto & sensor : sensors.gridSensors)
        {
            if (sensor->angle() < 0) { reading.leftNest = sensor->getReading(world); }
            if (sensor->angle() > 0) { reading.rightNest = sensor->getReading(world); }
            if (sensor->angle() == 0) { reading.midNest = sensor->getReading(world); }
        }
        for (auto & sensor : sensors.obstacleSensors)
        {
            if (sensor->angle() <= 0) { reading.leftObstacle += sensor->getReading(world); }
            if (sensor->angle() > 0) { reading.rightObstacle += sensor->getReading(world); }
        }
        for (auto & sensor : sensors.puckSensors)
        {
            if (sensor->angle() <= 0) { reading.leftPucks += sensor->getReading(world); }
            if (sensor->angle() > 0) { reading.rightPucks += sensor->getReading(world); }
        }
    }

    
}