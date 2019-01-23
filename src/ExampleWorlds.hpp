#pragma once

#include "Simulator.hpp"
#include "Robot.hpp"
#include "RobotAction.hpp"
#include "World.hpp"
#include "ExampleGrids.hpp"

namespace ExampleWorlds
{
    // populates a world with a grid of pucks
    World GetGridWorld1080(size_t skip)
    {
        World world(1920, 1080);
        world.addRobot(Vec2(200, 360), 50);

        for (size_t i = 0; i < 140; i += skip)
        {
            for (size_t j = 0; j < 90; j += skip)
            {
                Vec2 position(400.0f + (double)i * 10, 100.0f + (double)j * 10);
                double radius = skip * 4.0f;
                std::array<uint8_t, 4> color = { 0, 100, 200, 255 };
                world.addPuck(position, radius, color);
            }
        }

        return world;
    }

    World GetGridWorld720(size_t skip)
    {
        World world(1280, 720);

        world.addRobot(Vec2(200, 180), 40, { 44, 160, 44, 255 });
        world.getRobots()[0].addSensor(Sensor(30, 50, 0));
        world.getRobots()[0].addSensor(Sensor(-30, 50, 0));
        world.getRobots()[0].addSensor(Sensor(0, 50, 0));

        world.addRobot(Vec2(200, 540), 40, { 200, 44, 44, 255 });

        for (size_t i = 0; i < 80; i += skip)
        {
            for (size_t j = 0; j < 52; j += skip)
            {
                Vec2 position = Vec2(400.0f + (double)i * 10, 100.0f + (double)j * 10);
                double radius = skip * 4.0f;
                std::array<uint8_t, 4> color = { 0, 100, 200, 255 };
                world.addPuck(position, radius, { 0, 100, 200, 255 });
            }
        }

        world.addLine(Vec2(100, 260), Vec2(300, 260), 10);
        world.addLine(Vec2(100, 360), Vec2(300, 360), 10);
        world.addLine(Vec2(100, 460), Vec2(300, 460), 10);

        world.setGrid(ExampleGrids::GetInverseCenterDistanceGrid(64, 36));

        return world;
    }
};