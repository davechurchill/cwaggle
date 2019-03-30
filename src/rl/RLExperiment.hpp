#pragma once

#include <memory>
#include <fstream>
#include <string>
#include <functional>

#include "CWaggle.h"
#include "GUI.hpp"
#include "QLearning.hpp"
#include "Eval.hpp"
#include "OrbitalController.hpp"
#include "Hash.hpp"

struct RLExperimentConfig
{
    // Square World Parameters
    size_t width        = 800;
    size_t height       = 800;
    size_t gui          = 1;
    size_t numRobots    = 20;
    double robotRadius  = 10.0;
    size_t numPucks     = 250;
    double puckRadius   = 10.0;

    // Simulation Parameters
    double simTimeStep  = 1.0;
    double renderSteps  = 1;

    // Q-Learning Parameters
    size_t maxTimeSteps = 0;
    size_t numStates    = 0;
    size_t numActions   = 0;
    size_t batchSize    = 1;
    size_t qLearning    = 1;
    double initialQ     = 0.0;
    double alpha        = 0.1;
    double gamma        = 0.9;
    double epsilon      = 0.1;

    HashFunction hashFunction;

    size_t writePlotSkip    = 0;
    std::string plotFile   = "";
    size_t saveQSkip = 0;
    std::string saveQFile;
    size_t loadQ = 0;
    std::string loadQFile;
    double resetEval    = 0;

    std::vector<double> actions = { };

    // Orbital Construction Config
    OrbitalConstructionConfig occ;

    RLExperimentConfig() {}

    void load(const std::string & filename)
    {
        std::ifstream fin(filename);
        std::string token;
        double tempVal = 0;
        actions = {};

        while (fin.good())
        {
            fin >> token;
            if (token == "width")               { fin >> width; }
            else if (token == "height")         { fin >> height; }
            else if (token == "numRobots")      { fin >> numRobots; }
            else if (token == "gui")            { fin >> gui; }
            else if (token == "robotRadius")    { fin >> robotRadius; }
            else if (token == "numPucks")       { fin >> numPucks; }
            else if (token == "puckRadius")     { fin >> puckRadius; }
            else if (token == "simTimeStep")    { fin >> simTimeStep; }
            else if (token == "renderSkip")     { fin >> renderSteps; }
            else if (token == "forwardSpeed")   { fin >> occ.forwardSpeed; }
            else if (token == "angularSpeed")   { fin >> occ.maxAngularSpeed; }
            else if (token == "outieThreshold") { fin >> occ.thresholds[0]; }
            else if (token == "innieThreshold") { fin >> occ.thresholds[1]; }
            else if (token == "batchSize")      { fin >> batchSize; }
            else if (token == "maxTimeSteps")   { fin >> maxTimeSteps; }
            else if (token == "initialQ")       { fin >> initialQ; }
            else if (token == "alpha")          { fin >> alpha; }
            else if (token == "gamma")          { fin >> gamma; }
            else if (token == "epsilon")        { fin >> epsilon; }
            else if (token == "resetEval")      { fin >> resetEval; }
            else if (token == "writePlotSkip")  { fin >> writePlotSkip; }
            else if (token == "plotFilename")   { fin >> plotFile; }
            else if (token == "qLearning")      { fin >> qLearning; }
            else if (token == "savePolicy")     { fin >> saveQSkip >> saveQFile; }
            else if (token == "loadPolicy")     { fin >> loadQ >> loadQFile; }
            else if (token == "hashFunction")   
            { 
                fin >> token;
                hashFunction = Hash::GetHashData(token).Function; 
                numStates    = Hash::GetHashData(token).MaxHashSize;
            }
            else if (token == "actions") 
            { 
                fin >> numActions; 
                for (size_t a = 0; a < numActions; ++a)
                {
                    fin >> tempVal;
                    actions.push_back(tempVal);
                }
            }
        }
    }
};

class RLExperiment
{
    RLExperimentConfig          m_config;
    QLearning                   m_QL;

    std::shared_ptr<GUI>        m_gui;
    std::shared_ptr<Simulator>  m_sim;

    std::vector<Entity>         m_robotsActed;
    std::vector<size_t>         m_states;
    std::vector<size_t>         m_actions;
    std::vector<size_t>         m_nextStates;
    size_t                      m_stepsUntilRLUpdate = 1;

    size_t                      m_simulationSteps = 0;
    double                      m_simulationTime = 0;
    Timer                       m_simTimer;
    double                      m_previousEval = 0;
    size_t                      m_formations = 0;
    std::ofstream               m_fout;

    std::stringstream           m_status;


    // data keeping
    std::vector<size_t>         m_formationCompleteTimes;

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
        else if (m_config.gui)
        {
            m_gui = std::make_shared<GUI>(m_sim, 240);
        }
    }
    
    size_t getActionIndex(const EntityAction & action)
    {
        double closest = std::numeric_limits<double>().max();
        size_t closestAction = 0;
        for (size_t i = 0; i < m_config.actions.size(); i++)
        {
            double diff = std::abs(m_config.actions[i] - action.angularSpeed());
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
        return EntityAction(m_config.occ.forwardSpeed, m_config.actions[actionIndex]);
    }

public:

    RLExperiment(const RLExperimentConfig & config)
        : m_config(config)
    {
        m_QL = QLearning(m_config.numStates, m_config.numActions, m_config.alpha, m_config.gamma, m_config.initialQ);

        if (m_config.loadQ)
        {
            m_QL.load(m_config.loadQFile);
        }

        if (m_config.writePlotSkip)
        {
            m_fout = std::ofstream(m_config.plotFile);
        }

        resetSimulator();
        m_stepsUntilRLUpdate = m_config.batchSize;
    }
    
    void doSimulationStep()
    {
        if (m_config.writePlotSkip && m_simulationSteps % m_config.writePlotSkip == 0)
        {
            m_fout << m_simulationSteps << " " << m_previousEval << "\n";
            m_fout.flush();
        }
        
        if (m_config.saveQSkip && m_simulationSteps % m_config.saveQSkip == 0)
        {
            m_QL.save(m_config.saveQFile);
        }

        ++m_simulationSteps;

        if (!m_gui && (m_simulationSteps % 100000 == 0))
        {
            std::cout << "Simulation Step: " << m_simulationSteps << "\n";
        }

        SensorReading reading;

        // control robots that have controllers
        for (auto robot : m_sim->getWorld()->getEntities("robot"))
        {
            // record the robot sensor state into the batch
            SensorTools::ReadSensorArray(robot, m_sim->getWorld(), reading);
            m_states.push_back(m_config.hashFunction(reading));

            // get the action that should be done for this entity
            EntityAction action;

            // epsilon-greedy action selection
            if ((rand() / (double)RAND_MAX) < m_config.epsilon)
            {
                action = getAction(rand() % 4);
            }
            else
            {
                action = getAction(m_QL.selectActionFromPolicy(m_config.hashFunction(reading)));
                // action = EntityControllers::OrbitalConstruction(robot, m_sim->getWorld(), reading, m_config.occ);
            }

            // record the action that the robot did into the batch
            m_actions.push_back(getActionIndex(action));
            m_robotsActed.push_back(robot);

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
            m_nextStates.push_back(m_config.hashFunction(reading));
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

            if (m_config.qLearning)
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
            m_robotsActed.clear();
            m_stepsUntilRLUpdate = m_config.batchSize;
        }
    }

    void printResults()
    {
        // prints out the number of formations completed
        std::stringstream ss;
        ss << "gnuplot/results_form_" << m_config.numRobots << "_" << m_config.maxTimeSteps << ".txt";
        std::cout << "Printing Results to: " << ss.str() << "\n";

        std::ofstream fout(ss.str());
        fout << m_formations;

        // prints out the number of  completed
        std::stringstream ss2;
        ss2 << "gnuplot/results_form_over_time_" << m_config.numRobots << "_" << m_config.maxTimeSteps << ".txt";
        std::cout << "Printing Results to: " << ss2.str() << "\n";


        std::ofstream fout2(ss2.str());
        fout2 << "0 0\n";
        for (size_t i = 0; i < m_formationCompleteTimes.size(); i++)
        {
            fout2 << m_formationCompleteTimes[i] << " " << (i+1) << "\n";  
        }

    }

    void run()
    {
        bool running = true;
        while (running)
        {
            double eval = Eval::PuckAvgThresholdDiff(m_sim->getWorld(), m_config.occ.thresholds[0], m_config.occ.thresholds[1]);
            
            m_simTimer.start();
            for (size_t i = 0; i < m_config.renderSteps; i++)
            {
                if (m_config.maxTimeSteps > 0 && m_simulationSteps >= m_config.maxTimeSteps)
                {
                    running = false;
                }

                doSimulationStep();
            }
            m_simulationTime += m_simTimer.getElapsedTimeInMilliSec();

            if (m_gui)
            {
                // update gui status text
                m_status = std::stringstream();
                m_status << "Sim Steps:  " << m_simulationSteps << "\n";
                m_status << "Sim / Sec:  " << m_simulationSteps * 1000 / m_simulationTime << "\n";
                m_status << "QO Coverage: " << m_QL.getCoverage() << " of " << m_QL.size() << "\n";
                m_status << "Puck Eval:  " << eval << "\n";
                m_status << "Formations: " << m_formations << "\n";
                m_gui->setStatus(m_status.str());

                // draw gui
                m_gui->update();
            }

            if (m_config.resetEval && (eval > m_config.resetEval))
            {
                m_formations += 1;
                m_formationCompleteTimes.push_back(m_simulationSteps);
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

        RLExperiment exp(config);
        exp.run();
        exp.printResults();
    }
}

