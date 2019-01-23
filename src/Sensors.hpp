#pragma once

#include "Sensor.hpp"
#include "World.hpp"

#include <math.h>

namespace Sensors
{
    Vec2 GetSensorPosition(const Sensor & sensor, World & world)
    {
        const Robot & robot = world.getRobots()[sensor.robotID()];
        const Vec2 & pos    = world.getCircles()[robot.bodyID()].p;
        
        double absoluteAngle = sensor.angle() + robot.angle();
        //std::cout << absoluteAngle << "\n";
        
        Vec2 relativePos(sensor.radius() * cos(absoluteAngle), sensor.radius() * sin(absoluteAngle));

        return pos + relativePos;
    }
    
    double GetSensorReading(Sensor & sensor, World & world)
    {
        Robot & robot = world.getRobots()[sensor.robotID()];

        return 0;
    }
}