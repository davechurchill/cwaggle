#pragma once

#include "Simulator.hpp"
#include "EntityControllers.hpp"
#include "World.hpp"
#include "ExampleGrids.hpp"
#include "Sensors.hpp"

namespace orbital_av_world
{

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
            //sensors.puckSensors.push_back(std::make_shared<PuckSensor>(robot, 30, robotSize * 4, robotSize * 2));
            //sensors.puckSensors.push_back(std::make_shared<PuckSensor>(robot, 60, robotSize * 7, robotSize * 2));
            sensors.puckSensors.push_back(std::make_shared<PuckSensor>(robot, -60, robotSize * 7, robotSize * 2));
            sensors.obstacleSensors.push_back(std::make_shared<ObstacleSensor>(robot, 45, robotSize, robotSize/4));
            sensors.obstacleSensors.push_back(std::make_shared<ObstacleSensor>(robot, -45, robotSize, robotSize/4));
        }
/*
        // add the innie robots
        for (size_t r = 0; r < numRobots/2; r++)
        {
            Entity robot = world->addEntity("robot");
            Vec2 rPos(rand() % width, rand() % height);
            robot.addComponent<CTransform>(rPos);
            robot.addComponent<CCircleBody>(robotSize);
            robot.addComponent<CCircleShape>(robotSize);
            robot.addComponent<CColor>(44, 160, 44, 255);
            robot.addComponent<CRobotType>(1);

            auto & sensors = robot.addComponent<CSensorArray>();
            sensors.gridSensors.push_back(std::make_shared<GridSensor>(robot, 45, robotSize * 2));
            sensors.gridSensors.push_back(std::make_shared<GridSensor>(robot, 0, robotSize * 2));
            sensors.gridSensors.push_back(std::make_shared<GridSensor>(robot, -45, robotSize * 2));
            //sensors.puckSensors.push_back(std::make_shared<PuckSensor>(robot, -45, robotSize * 3, robotSize * 2));
            sensors.puckSensors.push_back(std::make_shared<PuckSensor>(robot, 45, robotSize * 3, robotSize * 2));
//            sensors.puckSensors.push_back(std::make_shared<PuckSensor>(robot, 70, robotSize * 6.5, robotSize * 2));
            //sensors.puckSensors.push_back(std::make_shared<PuckSensor>(robot, -70, robotSize * 6.5, robotSize * 2));
            sensors.obstacleSensors.push_back(std::make_shared<ObstacleSensor>(robot, 45, robotSize, robotSize/4));
            sensors.obstacleSensors.push_back(std::make_shared<ObstacleSensor>(robot, -45, robotSize, robotSize/4));
        }
*/
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
        
        //world->setGrid(ExampleGrids::GetInverseCenterDistanceGrid(64, 64));
        world->setGrid(ExampleGrids::GetGridFromFile("images/horse.png"));

        world->update();
        return world;
    }
};