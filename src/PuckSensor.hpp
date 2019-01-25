#pragma once

class PuckSensor
{
    double m_angle      = 0; 
    double m_distance   = 0;
    double m_radius     = 0;

public:

    PuckSensor() {}

    PuckSensor(double angle, double distance, double radius)
        : m_angle(angle*3.1415926/180.0), m_distance(distance), m_radius(radius) { }

    inline double distance() const
    {
        return m_distance;
    }

    inline double angle() const
    {
        return m_angle;
    }
};