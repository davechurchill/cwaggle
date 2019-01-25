#pragma once

class GridSensor
{
    double m_angle  = 0; 
    double m_radius = 0;

public:

    GridSensor() {}

    GridSensor(double angle, double radius)
        : m_angle(angle*3.1415926/180.0), m_radius(radius) { }

    inline double radius() const
    {
        return m_radius;
    }

    inline double angle() const
    {
        return m_angle;
    }

};