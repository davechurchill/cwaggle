#pragma once

#include "GridSensor.hpp"
#include "PuckSensor.hpp"
#include "World.hpp"

#include <math.h>

namespace SensorTools
{
    Vec2 GetGridSensorPosition(const GridSensor & sensor, World & world)
    {
        return { 0, 0 };

        /*const Robot & robot = world.getRobots()[sensor.robotID()];
        const Vec2 & pos    = world.getCircles()[robot.bodyID()].p;
        double sAngle       = sensor.angle() + robot.angle();
        return pos + Vec2(sensor.radius() * cos(sAngle), sensor.radius() * sin(sAngle));*/
    }
    
    double GetGridSensorReading(GridSensor & sensor, World & world)
    {
        /*if (world.getGrid().width() == 0) { return 0; }
        Vec2 sPos = GetGridSensorPosition(sensor, world);
        size_t gX = (size_t)round(sPos.x / world.width());
        size_t gY = (size_t)round(sPos.y / world.height());
        return world.getGrid().get(gX, gY);*/
        return 0;
    }
}