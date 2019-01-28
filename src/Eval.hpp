#pragma once

#include "World.hpp"
#include "Sensors.h"
#include "Entity.hpp"

namespace Eval
{
    double PuckClusterEval(std::shared_ptr<World> world)
    {
        // 1. calculate the average of all puck positions
        // 2. calculate sum of distances to the center
        // 3. return that sum

        for (auto e : world->getEntities("puck"))
        {
            auto & t = e.getComponent<CTransform>();
            auto & b = e.getComponent<CCircleBody>();


        }

        return 0;
    }
}