#pragma once

class RobotAction
{
    double  m_speed         = 0;
    double  m_angularSpeed  = 0;
    bool    m_gripperOn     = false;
    bool    m_flashOn       = false;
    bool    m_emitPheromone = false;
    
public:

    RobotAction() {}
    RobotAction(double speed, double angularSpeed)
        : m_speed(speed), m_angularSpeed(angularSpeed) { }

    inline const auto speed()        const { return m_speed; }
    inline const auto angularSpeed() const { return m_angularSpeed; }
};