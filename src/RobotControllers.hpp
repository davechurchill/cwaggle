#pragma once

#include "Simulator.hpp"
#include "Robot.hpp"
#include "RobotAction.hpp"

namespace RobotControllers
{
    RobotAction MoveStraightController(const Robot & robot, const Simulator & sim)
    {
        return RobotAction(20, 0);
    }
};