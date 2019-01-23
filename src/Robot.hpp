#pragma once

#include "RobotAction.hpp"
#include "Sensor.hpp"

#include <iostream>
#include <stdint.h>
#include <array>


class Robot
{
    size_t m_bodyID = 0;
    size_t m_id     = 0;
    double m_angle  = 0;
    double m_speed  = 0;
    double m_turn   = 0;
    std::array<uint8_t, 4> m_color = { 255, 0, 0, 255 };
    std::vector<Sensor> m_sensors;
    RobotAction m_action;
    bool m_doAction = false;

public:

    Robot() {}

    Robot(size_t id, size_t bodyID)
        : m_id(id), m_bodyID(bodyID) { }

    Robot(size_t id, size_t bodyID, decltype(m_color) color)
        : m_id(id), m_bodyID(bodyID), m_color(color) { }

    inline const auto   id()        const { return m_id; }
    inline const auto   bodyID()    const { return m_bodyID; }
    inline const auto   speed()     const { return m_speed; }
    inline const auto   angle()     const { return m_angle; }
    inline const auto   color()     const { return m_color; }
    inline const auto   doAction()  const { return m_doAction; }
    inline const auto & action()    const { return m_action; }
    inline const auto & sensors()   const { return m_sensors; }

    inline void addSensor(const Sensor & sensor)
    {
        m_sensors.push_back(sensor);
    }

    inline void setColor(decltype(m_color) color) 
    { 
        m_color = color; 
    }

    inline void doAction(const RobotAction & action)
    {
        m_doAction = true;
        m_action = action;

        // update the robot's speed and angle based on the action
        m_angle += action.angularSpeed();
        m_speed = action.speed();
    }

};