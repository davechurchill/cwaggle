#pragma once

#include "Simulator.hpp"
#include "EntityControllers.hpp"
#include "World.hpp"
#include "ExampleGrids.hpp"
#include "Sensors.hpp"

namespace ExampleWorlds
{
    std::shared_ptr<World> GetGridWorld1080(size_t skip)
    {
        auto world = std::make_shared<World>(1920, 1080);
        
        Entity robot1 = world->addEntity("robot");
        robot1.addComponent<CTransform>(Vec2(200, 300));
        robot1.addComponent<CCircleBody>(30);
        robot1.addComponent<CCircleShape>(30);
        robot1.addComponent<CColor>(0, 100, 200, 255);

        Entity robot2 = world->addEntity("robot");
        robot2.addComponent<CTransform>(Vec2(200, 800));
        robot2.addComponent<CCircleBody>(30);
        robot2.addComponent<CCircleShape>(30);
        robot2.addComponent<CColor>(44, 160, 44, 255);

        for (size_t i = 0; i < 140; i += skip)
        {
            for (size_t j = 0; j < 90; j += skip)
            {                
                Entity puck = world->addEntity("puck");
                puck.addComponent<CTransform>(Vec2(400.0 + i * 10, 100.0 + j * 10));
                puck.addComponent<CCircleBody>(skip * 4.0);
                puck.addComponent<CCircleShape>(skip * 4.0);
                puck.addComponent<CColor>(200, 44, 44, 255);
            }
        }

        // add some lines
        for (size_t i = 0; i < 3; i++)
        {
            Entity line = world->addEntity("line");
            line.addComponent<CLineBody>(Vec2(100, 450 + i * 100), Vec2(300, 450 + i * 100), 10);
        }

        world->update();
        return world;
    }

    std::shared_ptr<World> GetGridWorld720(size_t skip)
    {
        auto world = std::make_shared<World>(1280, 720);

        Entity robot1 = world->addEntity("robot");
        robot1.addComponent<CTransform>(Vec2(200, 200));
        robot1.addComponent<CCircleBody>(40);
        robot1.addComponent<CCircleShape>(40);
        robot1.addComponent<CColor>(0, 100, 200, 255);
        
        Entity robot2 = world->addEntity("robot");
        robot2.addComponent<CTransform>(Vec2(200, 600));
        robot2.addComponent<CCircleBody>(50);
        robot2.addComponent<CCircleShape>(50);
        robot2.addComponent<CColor>(44, 160, 44, 255);

        for (size_t i = 0; i < 80; i += skip)
        {
            for (size_t j = 0; j < 52; j += skip)
            {
                Entity puck = world->addEntity("puck");
                puck.addComponent<CTransform>(Vec2(400.0 + i * 10, 100.0 + j * 10));
                puck.addComponent<CCircleBody>(skip * 4.0);
                puck.addComponent<CCircleShape>(skip * 4.0);
                puck.addComponent<CColor>(200, 44, 44, 255);
            }
        }

        // add some lines
        for (size_t i = 0; i < 3; i++)
        {
            Entity line = world->addEntity("line");
            line.addComponent<CLineBody>(Vec2(100, 300 + i * 100), Vec2(300, 300 + i * 100), 10);
        }

        world->update();
        return world;
    }

    std::shared_ptr<World> GetGetSquareWorld(size_t width, size_t height, size_t numRobots, double robotSize, size_t numPucks, double puckSize)
    {
        auto world = std::make_shared<World>(width, height);

        // add the outie robots
        for (size_t r = 0; r < numRobots; r++)
        {
            Entity robot = world->addEntity("robot");
            Vec2 rPos(rand() % width, rand() % height);
            robot.addComponent<CTransform>(rPos);
            robot.addComponent<CCircleBody>(robotSize);
            robot.addComponent<CCircleShape>(robotSize);
            robot.addComponent<CColor>(0, 100, 200, 255);
            robot.addComponent<CRobotType>(0);

            auto & sensors = robot.addComponent<CSensorArray>();
            sensors.gridSensors.push_back(std::make_shared<GridSensor>(robot, 45, robotSize * 2));
            sensors.gridSensors.push_back(std::make_shared<GridSensor>(robot, 0, robotSize * 2));
            sensors.gridSensors.push_back(std::make_shared<GridSensor>(robot, -45, robotSize * 2));
            sensors.puckSensors.push_back(std::make_shared<PuckSensor>(robot, -30, robotSize * 4, robotSize * 2));
            sensors.puckSensors.push_back(std::make_shared<PuckSensor>(robot, 30, robotSize * 4, robotSize * 2));
            sensors.puckSensors.push_back(std::make_shared<PuckSensor>(robot, 60, robotSize * 7, robotSize * 2));
            sensors.puckSensors.push_back(std::make_shared<PuckSensor>(robot, -60, robotSize * 7, robotSize * 2));
            sensors.obstacleSensors.push_back(std::make_shared<ObstacleSensor>(robot, 45, robotSize, robotSize/4));
            sensors.obstacleSensors.push_back(std::make_shared<ObstacleSensor>(robot, -45, robotSize, robotSize/4));
        }

        // add the pucks
        for (size_t r = 0; r < numPucks; r++)
        {
            int rWidth = rand() % (int)(width - 8 * puckSize);
            int rHeight = rand() % (int)(height - 8 * puckSize);
            Vec2 pPos(4*puckSize + rWidth, 4*puckSize + rHeight);

            Entity puck = world->addEntity("puck");
            puck.addComponent<CTransform>(pPos);
            puck.addComponent<CCircleBody>(puckSize);
            puck.addComponent<CCircleShape>(puckSize);
            puck.addComponent<CColor>(200, 44, 44, 255);
        }
        
        world->setGrid(ExampleGrids::GetInverseCenterDistanceGrid(64, 64));
        //world->setGrid(ValueGrid("triangle.png"));

        world->update();
        return world;
    }
};