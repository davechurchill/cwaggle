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

    WorldECS GetWorldECS(size_t skip)
    {
        WorldECS world(1920, 1080);
        
        Entity robot1 = world.addEntity("robot");
        robot1.addComponent<CTransform>(Vec2(200, 360));
        robot1.addComponent<CBody>(50);
        robot1.addComponent<CColor>(0, 100, 200, 255);

        Entity robot2 = world.addEntity("robot");
        robot2.addComponent<CTransform>(Vec2(200, 600));
        robot2.addComponent<CBody>(50);
        robot2.addComponent<CColor>(44, 160, 44, 255);

        for (size_t i = 0; i < 140; i += skip)
        {
            for (size_t j = 0; j < 90; j += skip)
            {                
                Entity puck = world.addEntity("puck");
                puck.addComponent<CTransform>(Vec2(400.0 + i * 10, 100.0 + j * 10));
                puck.addComponent<CBody>(skip * 4.0);
                puck.addComponent<CColor>(200, 44, 44, 255);
            }
        }

        return world;
    }

    World GetGridWorld720(size_t skip)
    {
        World world(1280, 720);

        world.addRobot(Vec2(200, 180), 40, { 44, 160, 44, 255 });
        world.getRobots()[0].addGridSensor(GridSensor(30, 50, 0));
        world.getRobots()[0].addGridSensor(GridSensor(-30, 50, 0));
        world.getRobots()[0].addGridSensor(GridSensor(0, 50, 0));

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

        world.setGrid(ExampleGrids::GetInverseCenterDistanceGrid(128, 72));

        return world;
    }

    World GetGetSquareWorld(Vec2 size, size_t numRobots, double robotSize, size_t numPucks, double puckSize)
    {
        World world(size.x, size.y);

        for (size_t r = 0; r < numRobots; r++)
        {
            Vec2 rPos(rand() % (int)size.x, rand() % (int)size.y);
            world.addRobot(rPos, robotSize, { 0, 100, 200, 255 });
            world.getRobots().back().setType(RobotTypes::Outie);

            size_t robotID = world.getRobots().back().id();
            world.getRobots().back().addGridSensor(GridSensor( 45, robotSize + 4, robotID));
            world.getRobots().back().addGridSensor(GridSensor(-45, robotSize + 4, robotID));
            world.getRobots().back().addGridSensor(GridSensor(  0, robotSize + 4, robotID));
        }

        for (size_t r = 0; r < numRobots/2; r++)
        {
            Vec2 rPos(rand() % (int)size.x, rand() % (int)size.y);
            world.addRobot(rPos, robotSize, { 44, 160, 44, 255 });
            world.getRobots().back().setType(RobotTypes::Innie);

            size_t robotID = world.getRobots().back().id();
            world.getRobots().back().addGridSensor(GridSensor(45, robotSize + 4, robotID));
            world.getRobots().back().addGridSensor(GridSensor(-45, robotSize + 4, robotID));
            world.getRobots().back().addGridSensor(GridSensor(0, robotSize + 4, robotID));
        }

        for (size_t r = 0; r < numPucks; r++)
        {
            int rWidth = rand() % (int)(size.x - 8 * puckSize);
            int rHeight = rand() % (int)(size.y - 8 * puckSize);
            Vec2 pPos(4*puckSize + rWidth, 4*puckSize + rHeight);
            world.addPuck(pPos, puckSize, { 200, 44, 44, 255 });
        }
        
        world.setGrid(ExampleGrids::GetInverseCenterDistanceGrid(64, 64));

        return world;
    }
};