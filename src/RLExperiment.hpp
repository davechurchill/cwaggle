#pragma once

#include <memory>
#include <fstream>
#include "GUI.hpp"
#include "SensorTools.hpp"
#include "QLearning.hpp"

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
    size_t batchSize    = 1;
    double initialQ     = 0.0;
    double alpha        = 0.1;
    double gamma        = 0.9;

    std::vector<double> angles = { };

    // Orbital Construction Config
    OrbitalConstructionConfig occ;

    RLExperimentConfig() {}

    void load(const std::string & filename)
    {
        std::ifstream fin(filename);
        std::string token;

        while (fin.good())
        {
            fin >> token;
            if (token == "width")               { fin >> width; }
            else if (token == "height")         { fin >> height; }
            else if (token == "numRobots")      { fin >> numRobots; }
            else if (token == "robotRadius")    { fin >> robotRadius; }
            else if (token == "numPucks")       { fin >> numPucks; }
            else if (token == "puckRadius")     { fin >> puckRadius; }
            else if (token == "simTimeStep")    { fin >> simTimeStep; }
            else if (token == "renderSkip")     { fin >> renderSteps; }
            else if (token == "forwardSpeed")   { fin >> occ.forwardSpeed; }
            else if (token == "angularSpeed")   { fin >> occ.maxAngularSpeed; }
            else if (token == "outieThreshold") { fin >> occ.thresholds[0]; }
            else if (token == "innieThreshold") { fin >> occ.thresholds[1]; }
            else if (token == "numStates")      { fin >> numStates; }
            else if (token == "numActions")     { fin >> numActions; }
            else if (token == "batchSize")      { fin >> batchSize; }
            else if (token == "initialQ")       { fin >> initialQ; }
            else if (token == "alpha")          { fin >> alpha; }
            else if (token == "gamma")          { fin >> gamma; }
        }
    }
};

size_t ComputeHash(SensorReading & reading)
{
    size_t sum = 0;

    sum += (reading.leftPucks == 0) ? 0 : (1 << 0);
    sum += (reading.rightPucks == 0) ? 0 : (1 << 1);
    sum += (reading.leftObstacle == 0) ? 0 : (1 << 2);
    sum += (reading.rightObstacle == 0) ? 0 : (1 << 3);
    sum += (size_t)(reading.leftNest * 15) * (1 << 4);
    sum += (size_t)(reading.midNest * 15) * (1 << 8);
    sum += (size_t)(reading.rightNest * 15) * (1 << 12);

    if (sum >= (1 << 16)) { std::cout << "HASH ERROR: " << sum << "\n"; }
    return sum;
}


class RLExperiment
{
    RLExperimentConfig          m_config;
    QLearning                   m_QL;

    std::shared_ptr<GUI>        m_gui;
    std::shared_ptr<Simulator>  m_sim;

    std::vector<size_t>         m_states;
    std::vector<size_t>         m_actions;
    std::vector<size_t>         m_nextStates;
    size_t                      m_stepsUntilRLUpdate = 1;

    void init(std::shared_ptr<World> world)
    {
        m_sim = std::make_shared<Simulator>(world);
        m_gui = std::make_shared<GUI>(m_sim, 240);
    }
    
    size_t getActionIndex(const EntityAction & action)
    {
        double closest = std::numeric_limits<double>().max();
        size_t closestAction = 0;
        for (size_t i = 0; i < m_config.angles.size(); i++)
        {
            double diff = abs(m_config.angles[i] - action.angularSpeed());
            if (diff < closest)
            {
                closest = diff;
                closestAction = 0;
            }
        }
        return closestAction;
    }

    EntityAction getAction(size_t actionIndex)
    {
        return EntityAction(m_config.occ.forwardSpeed, m_config.angles[actionIndex]);
    }

public:

    RLExperiment(const RLExperimentConfig & config)
        : m_config(config)
    {
        m_QL = QLearning(m_config.numStates, m_config.numActions, m_config.alpha, m_config.gamma, m_config.initialQ);

        init(ExampleWorlds::GetGetSquareWorld
        (
            m_config.width,     m_config.height,
            m_config.numRobots, m_config.robotRadius,
            m_config.numPucks,  m_config.puckRadius
        ));

        m_stepsUntilRLUpdate = m_config.batchSize;
    }
    
    void doSimulationStep()
    {
        SensorReading reading;

        // control robots that have controllers
        for (auto & robot : m_sim->getWorld()->getEntities("robot"))
        {
            // record the robot sensor state into the batch
            SensorTools::ReadSensorArray(robot, m_sim->getWorld(), reading);
            m_states.push_back(ComputeHash(reading));

            // get the action that should be done for this entity
            EntityAction action = EntityControllers::OrbitalConstruction(robot, m_sim->getWorld(), reading, m_config.occ);

            // record the action that the robot did into the batch
            m_actions.push_back(getActionIndex(action));

            // have the action apply its effects to the entity
            action.doAction(robot, m_config.simTimeStep);
        }

        // call the world physics simulation update
        // parameter = how much sim time should pass (default 1.0)
        m_sim->update(m_config.simTimeStep);

        // record the robot next states to the batch
        for (auto & robot : m_sim->getWorld()->getEntities("robot"))
        {
            // record the robot sensor state into the batch
            SensorTools::ReadSensorArray(robot, m_sim->getWorld(), reading);
            m_nextStates.push_back(ComputeHash(reading));
        }

        // if the batch size has been reached, do the update
        if (--m_stepsUntilRLUpdate == 0)
        {
            // TODO: calculate the reward
            double reward = Eval::PuckAvgThresholdDiff(m_sim->getWorld(), m_config.occ.thresholds[0], m_config.occ.thresholds[1]);

            // perform the Q-learning update
            for (size_t i = 0; i < m_states.size(); i++)
            {
                m_QL.updateValue(m_states[i], m_actions[i], reward, m_nextStates[i]);
                m_QL.updatePolicy(m_states[i]);
            }
            
            // clean up the data structures for next batch
            m_states.clear();
            m_actions.clear();
            m_nextStates.clear();
            m_stepsUntilRLUpdate = m_config.batchSize;
        }
    }

    void run()
    {
        while (true)
        {
            for (size_t i = 0; i < m_config.renderSteps; i++)
            {
                doSimulationStep();
            }

            if (m_gui)
            {
                m_gui->update();
            }
        }
    }
};


namespace RLExperiments
{
    void MainRLExperiment(int argc, char ** argv)
    {
        RLExperimentConfig config;
        config.load("rl_config.txt");
        
        config.angles =
        {
             config.occ.maxAngularSpeed,
             config.occ.maxAngularSpeed / 3,
            -config.occ.maxAngularSpeed / 3,
            -config.occ.maxAngularSpeed
        };

        RLExperiment exp(config);
        exp.run();
    }
}