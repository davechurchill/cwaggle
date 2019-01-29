#pragma once

#include <memory>
#include "GUI.hpp"

struct RLExperimentConfig
{
    // Square World Parameters
    size_t width        = 800;
    size_t height       = 800;
    size_t numRobots    = 20;
    double robotRadius  = 10.0;
    size_t numPucks     = 250;
    double puckRadius   = 10.0;

    // Simulation Parameters
    double simTimeStep  = 1.0;
    double renderSteps  = 1;

    // Q-Learning Parameters
    size_t numStates    = 0;
    size_t numActions   = 0;
    double initialQ     = 0.0;
    double alpha        = 0.1;
    double gamma        = 0.9;
};

class RLExperiment
{
    RLExperimentConfig          m_config;
    
    std::shared_ptr<GUI>        m_gui;
    std::shared_ptr<Simulator>  m_sim;


    void init(std::shared_ptr<World> world)
    {
        m_sim = std::make_shared<Simulator>(world);
        m_gui = std::make_shared<GUI>(m_sim, 240);
    }

public:

    RLExperiment(RLExperimentConfig config)
        : m_config(config)
    {
        init(ExampleWorlds::GetGetSquareWorld
        (
            m_config.width,     m_config.height, 
            m_config.numRobots, m_config.robotRadius, 
            m_config.numPucks,  m_config.puckRadius
        ));
    }
    
    void run()
    {
        while (true)
        {
            for (size_t i = 0; i < m_config.renderSteps; i++)
            {
                // control robots that have controllers
                for (auto & robot : m_sim->getWorld()->getEntities("robot"))
                {
                    // if the entity doesn't have a controller we can skip it
                    if (!robot.hasComponent<CController>()) { continue; }

                    // get the action that should be done for this entity
                    EntityAction action = robot.getComponent<CController>().controller->getAction();

                    // have the action apply its effects to the entity
                    action.doAction(robot, m_config.simTimeStep);
                }

                // call the world physics simulation update
                // parameter = how much sim time should pass (default 1.0)
                m_sim->update(m_config.simTimeStep);
            }

            // if a gui exists, call for its display to update
            // note: simulation is limited by gui frame rate limit
            if (m_gui)
            {
                m_gui->update();
            }
        }
    }
};


namespace RLExperiments
{
    void MainRLExperiment()
    {
        RLExperimentConfig config;

        // World Properties
        config.width = 800;
        config.height = 800;
        config.numRobots = 20;
        config.robotRadius = 10.0;
        config.numPucks = 250;
        config.puckRadius = 10.0;

        // Simulation parameters
        config.simTimeStep = 1.0;
        config.renderSteps = 1;

        // Q-Learning Parameters
        config.numStates = 0;
        config.numActions = 0;
        config.initialQ = 0.0;
        config.alpha = 0.1;
        config.gamma = 0.9;

        RLExperiment exp(config);
        exp.run();
    }
}