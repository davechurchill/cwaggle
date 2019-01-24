#pragma once

#include "RobotAction.hpp"
#include "GridSensor.hpp"
#include "PuckSensor.hpp"
#include <iostream>
#include <stdint.h>
#include <array>

namespace RobotTypes
{
    enum { Innie, Outie };
}

class Robot
{
    size_t m_bodyID = 0;
    size_t m_id     = 0;
    size_t m_type   = 0;
    double m_angle  = 0;
    double m_speed  = 0;
    double m_turn   = 0;
    std::array<uint8_t, 4> m_color = { 255, 0, 0, 255 };
    std::vector<GridSensor> m_gridSensors;
    std::vector<PuckSensor> m_puckSensors;
    RobotAction m_action;
    bool m_doAction = false;

public:

    Robot() {}

    Robot(size_t id, size_t bodyID)
        : m_id(id), m_bodyID(bodyID) { }

    Robot(size_t id, size_t bodyID, decltype(m_color) color)
        : m_id(id), m_bodyID(bodyID), m_color(color) { }

    inline const auto   id()        const { return m_id; }
    inline const auto   type()      const { return m_type; }
    inline const auto   bodyID()    const { return m_bodyID; }
    inline const auto   speed()     const { return m_speed; }
    inline const auto   angle()     const { return m_angle; }
    inline const auto   color()     const { return m_color; }
    inline const auto   doAction()  const { return m_doAction; }
    inline const auto & action()    const { return m_action; }
    inline const auto & gridSensors() const { return m_gridSensors; }
    inline const auto & puckSensors() const { return m_puckSensors; }

    inline void setType(size_t type)
    {
        m_type = type;
    }

    inline void addGridSensor(const GridSensor & sensor)
    {
        m_gridSensors.push_back(sensor);
    }

    inline void addPuckSensor(const PuckSensor & sensor)
    {
        m_puckSensors.push_back(sensor);
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