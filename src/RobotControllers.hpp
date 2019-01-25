#pragma once

#include "Simulator.hpp"
#include "RobotAction.hpp"

namespace RobotControllers
{
    RobotAction TurnController(double speed, double angle)
    {
        return RobotAction(speed, angle);
    }
};