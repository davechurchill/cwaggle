#pragma once

#include <vector>
#include <algorithm>
#include <fstream>

class QLearning 
{
    size_t m_numStates  = 0;
    size_t m_numActions = 0;
    size_t m_updates    = 0;
    size_t m_visited    = 0;
    double m_alpha      = 0;
    double m_gamma      = 0;
    double m_initialQ   = 0;
    double m_diffThresh = 0;

    std::vector<std::vector<double>> m_Q;
    std::vector<std::vector<double>> m_P;
    std::vector<std::vector<size_t>> m_N;

    std::vector<size_t> m_maxActions;

public:

    QLearning() {}

    QLearning(size_t numStates, size_t numActions, double alpha, double gamma, double initialQ) 
        : m_numStates   (numStates)
        , m_numActions  (numActions)
        , m_alpha       (alpha)
        , m_gamma       (gamma)
        , m_initialQ    (initialQ)
        , m_Q           (numStates, std::vector<double>(numActions, initialQ))
        , m_P           (numStates, std::vector<double>(numActions, 1.0 / numActions))
        , m_N           (numStates, std::vector<size_t>(numActions, 0))
    {
     
    }

    void save(const std::string & filename)
    {
        std::ofstream fout(filename);
        fout << m_numStates << " " << m_numActions << " " << m_updates << " " << m_alpha << " " << m_gamma << " " << m_initialQ << " " << m_updates << " " << m_visited << " \n";

        for (size_t s = 0; s < m_numStates; s++)
        {
            for (size_t a = 0; a < m_numActions; a++)
            {
                fout << m_Q[s][a] << " " << m_N[s][a] << " " << m_P[s][a] << " ";
            }
        }
    }

    void load(const std::string & filename)
    {
        std::ifstream fin(filename);
        fin >> m_numStates >> m_numActions >> m_updates >> m_alpha >> m_gamma >> m_initialQ >> m_updates >> m_visited;

        for (size_t s = 0; s < m_numStates; s++)
        {
            for (size_t a = 0; a < m_numActions; a++)
            {
                fin >> m_Q[s][a] >> m_N[s][a] >> m_P[s][a];
            }
        }
    }

    // Select an action from our policy at a given state s
    size_t selectActionFromPolicy(size_t s)
    {
        double maxQ = *std::max_element(m_Q[s].begin(), m_Q[s].end());

        m_maxActions.clear();
        for (size_t a=0; a<m_Q[s].size(); a++) {
            if (m_Q[s][a] == maxQ) {
                m_maxActions.push_back(a); 
            }
        }
        // return a random action from the maximums
        return m_maxActions[rand() % m_maxActions.size()];
    }

    size_t selectMostChosenAction(size_t s) 
    {
        auto maxVisits = std::max_element(m_N[s].begin(), m_N[s].end());
        if (*maxVisits == 0) { std::cout << "Warning, state unvisited: " << s << "\n"; }
        return maxVisits - m_N[s].begin();
    }

    // Update the value estimate of Q[s][a] based on a given sample
    // Note: s and a must be integer hash of state and action
    void updateValue(size_t s, size_t a, double r, size_t ns) 
    {
        ++m_updates;
        if (m_N[s][a] == 0) { m_visited++; }
        ++m_N[s][a];
        double maxNSQ = *std::max_element(m_Q[ns].begin(), m_Q[ns].end());
        m_Q[s][a] += m_alpha * (r + m_gamma*maxNSQ - m_Q[s][a]);
    }

    void updatePolicy(size_t s) {
        
        // find the maximum value of any action at the given state
        double maxVal = *std::max_element(m_Q[s].begin(), m_Q[s].end());

        // record all of the actions which have the max value
        m_maxActions.clear();
        for (size_t a = 0; a < m_Q[s].size(); ++a) {
            if (fabs(m_Q[s][a] - maxVal) < m_diffThresh) {
                m_maxActions.push_back(a);
            }
        }

        std::fill(m_P[s].begin(), m_P[s].end(), 0.0);

        // set all max action value propabilities accordingly
        for (size_t a=0; a<m_maxActions.size(); ++a) {
            m_P[s][m_maxActions[a]] = 1.0 / m_maxActions.size();
        }
    }

    size_t size() const
    {
        return m_numStates * m_numActions;
    }

    double getCoverage() const
    {
        return (double)m_visited / (m_numStates * m_numActions);
    }

    size_t numUpdates() const
    {
        return m_updates;
    }
};