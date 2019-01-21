#pragma once

#include "Simulator.hpp"
#include "Robot.hpp"
#include "RobotAction.hpp"
#include "World.hpp"

namespace ExampleWorlds
{
    // populates a world with a grid of pucks
    std::shared_ptr<World> GetGridWorld1080(size_t skip)
    {
        auto world = std::make_shared<World>(1920, 1080);
        world->addRobot(Vec2(200, 360), 50);

        for (size_t i = 0; i < 140; i += skip)
        {
            for (size_t j = 0; j < 90; j += skip)
            {
                world->addPuck(Vec2(400.0f + (double)i * 10, 100.0f + (double)j * 10), skip*4.0f);
            }
        }

        return world;
    }

    std::shared_ptr<World> GetGridWorld720(size_t skip)
    {
        auto world = std::make_shared<World>(1280, 720);
        world->addRobot(Vec2(200, 360), 40);

        for (size_t i = 0; i < 80; i += skip)
        {
            for (size_t j = 0; j < 52; j += skip)
            {
                world->addPuck(Vec2(400.0f + (double)i * 10, 100.0f + (double)j * 10), skip*4.0f);
            }
        }

        return world;
    }
};