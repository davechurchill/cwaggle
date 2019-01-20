#pragma once

#include "Simulator.hpp"
#include "Robot.hpp"
#include "RobotAction.hpp"
#include "World.hpp"

namespace ExampleWorlds
{
    World GetGridWorld(size_t skip)
    {
        World world(1920, 1080);
        world.addRobot(Vec2(200, 360), 50);

        for (size_t i = 0; i < 140; i += skip)
        {
            for (size_t j = 0; j < 90; j += skip)
            {
                world.addPuck(Vec2(400.0f + (double)i * 10, 100.0f + (double)j * 10), skip*4.0f);
            }
        }

        return world;
    }
};