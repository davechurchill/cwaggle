#pragma once

class RobotAction
{
    double  m_linearSpeed   = 0;
    double  m_angularSpeed  = 0;
    bool    m_gripperOn     = false;
    bool    m_flashOn       = false;
    bool    m_emitPheromone = false;
    
public:

    RobotAction() {}

    RobotAction(double linearSpeed, double angularSpeed)
        : m_linearSpeed(linearSpeed)
        , m_angularSpeed(angularSpeed)
    {

    }

    inline double linearSpeed() const
    {
        return m_linearSpeed;
    }

    inline double angularSpeed() const
    {
        return m_angularSpeed;
    }
};