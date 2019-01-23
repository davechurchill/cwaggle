#pragma once

#include "CircleBody.hpp"

class Sensor
{
    size_t m_robotID = 0;
    double m_angle  = 0; 
    double m_radius = 0;

public:

    Sensor() {}

    Sensor(double angle, double radius, size_t robotID) 
        : m_angle(angle*3.1415926/180.0), m_radius(radius), m_robotID(robotID) { }

    inline double radius() const
    {
        return m_radius;
    }

    inline double angle() const
    {
        return m_angle;
    }

    inline size_t robotID() const
    {
        return m_robotID;
    }
};