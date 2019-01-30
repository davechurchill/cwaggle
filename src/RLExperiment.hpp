#pragma once

#include <memory>
#include <fstream>
#include "GUI.hpp"
#include "SensorTools.hpp"
#include "QLearning.hpp"
#include "Eval.hpp"
#include "Timer.hpp"

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

    sum += (reading.leftPucks == 0)             ? 0 : (1 << 0);
    sum += (reading.rightPucks == 0)            ? 0 : (1 << 1);
    sum += (reading.leftObstacle == 0)          ? 0 : (1 << 2);
    sum += (reading.rightObstacle == 0)         ? 0 : (1 << 3);
    sum += (size_t)(reading.midNest * 15)           * (1 << 4);
    sum += (reading.leftNest < reading.midNest) ? 0 : (1 << 8);
    sum += (reading.rightNest < reading.midNest) ? 0 : (1 << 9);

    if (sum >= (1 << 10)) { std::cout << "HASH ERROR: " << sum << "\n"; }
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

    size_t                      m_simulationSteps = 0;
    double                      m_simulationTime = 0;
    Timer                       m_simTimer;
    double                      m_previousEval = 0;

    std::stringstream           m_status;

    void resetSimulator()
    {
        auto world = ExampleWorlds::GetGetSquareWorld
        (
            m_config.width, m_config.height,
            m_config.numRobots, m_config.robotRadius,
            m_config.numPucks, m_config.puckRadius
        );

        m_sim = std::make_shared<Simulator>(world);

        m_previousEval = Eval::PuckAvgThresholdDiff(m_sim->getWorld(), m_config.occ.thresholds[0], m_config.occ.thresholds[1]);
        
        if (m_gui)
        {
            m_gui->setSim(m_sim);
        }
        else
        {
            m_gui = std::make_shared<GUI>(m_sim, 240);
        }
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
                closestAction = i;
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

        resetSimulator();
        

        m_stepsUntilRLUpdate = m_config.batchSize;
    }
    
    void doSimulationStep()
    {
        ++m_simulationSteps;
        SensorReading reading;

        // control robots that have controllers
        for (auto & robot : m_sim->getWorld()->getEntities("robot"))
        {
            // record the robot sensor state into the batch
            SensorTools::ReadSensorArray(robot, m_sim->getWorld(), reading);
            m_states.push_back(ComputeHash(reading));

            // get the action that should be done for this entity
            EntityAction action;
            //action = EntityControllers::OrbitalConstruction(robot, m_sim->getWorld(), reading, m_config.occ);
            action = getAction(m_QL.selectActionFromPolicy(ComputeHash(reading)));

            if (reading.leftNest == 0 || reading.rightNest == 0 || reading.midNest == 0)
            {
                //action = getAction(0);
            }

            if (rand() / (double)RAND_MAX < 0.2)
            {
                action = getAction(rand() % 4);
            }

            /*if (m_simulationSteps < 50000)
            {
                action = EntityControllers::OrbitalConstruction(robot, m_sim->getWorld(), reading, m_config.occ);
            }
            else
            {
                if (rand() / (double)RAND_MAX < 0.2)
                {
                    action = getAction(rand() % 4);
                }
                else
                {
                    action = getAction(m_QL.selectMostChosenAction(ComputeHash(reading)));
                }
            }*/

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

        if (m_states.size() != m_actions.size() || m_states.size() != m_nextStates.size())
        {
            std::cout << "Warning: Batch Size Mismatch: S " << m_states.size() << " A " << m_actions.size() << " NS " << m_nextStates.size() << "\n";
        }

        // if the batch size has been reached, do the update
        if (--m_stepsUntilRLUpdate == 0)
        {
            // TODO: calculate the reward
            double eval = Eval::PuckAvgThresholdDiff(m_sim->getWorld(), m_config.occ.thresholds[0], m_config.occ.thresholds[1]);
            double reward = eval - m_previousEval;

            if (reward <= 0) reward -= 1;

            // perform the Q-learning update
            //if (reward != 0)
            {
                for (size_t i = 0; i < m_states.size(); i++)
                {
                    m_QL.updateValue(m_states[i], m_actions[i], reward, m_nextStates[i]);
                    m_QL.updatePolicy(m_states[i]);
                }
            }

            m_previousEval = eval;
            
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
            double eval = Eval::PuckAvgThresholdDiff(m_sim->getWorld(), m_config.occ.thresholds[0], m_config.occ.thresholds[1]);
            
            m_simTimer.start();
            for (size_t i = 0; i < m_config.renderSteps; i++)
            {
                doSimulationStep();
            }
            m_simulationTime += m_simTimer.getElapsedTimeInMilliSec();

            if (m_gui)
            {
                // update gui status text
                m_status = std::stringstream();
                m_status << "Sim Steps:  " << m_simulationSteps << "\n";
                m_status << "Sim / Sec:  " << m_simulationSteps * 1000 / m_simulationTime << "\n";
                m_status << "Q Coverage: " << m_QL.getCoverage() << "\n";
                m_status << "Puck Eval:  " << eval << "\n";
                m_gui->setStatus(m_status.str());

                // draw gui
                m_gui->update();
            }

            if (eval > 0.92)
            {
                resetSimulator();
            }
        }
    }
};


namespace RLExperiments
{
    void MainRLExperiment()
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