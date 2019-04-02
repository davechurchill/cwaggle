#pragma once

#include "World.hpp"
#include "Sensors.hpp"
#include "Entity.hpp"

namespace Eval
{
    double PuckCenterSSD(std::shared_ptr<World> world)
    {
        Vec2 averagePosition;

        for (auto e : world->getEntities("puck"))
        {
            auto & t = e.getComponent<CTransform>();

            averagePosition += t.p;
        }

        averagePosition /= world->getEntities("puck").size();

        double ssd = 0;

        for (auto e : world->getEntities("puck"))
        {
            ssd += e.getComponent<CTransform>().p.dist(averagePosition);
        }

        return 800 - (ssd / world->getEntities("puck").size());
    }

    double PuckAvgThresholdDiff(std::shared_ptr<World> world, double t1, double t2)
    {
        double sum = 0;
        auto & grid = world->getGrid();
        for (auto e : world->getEntities("puck"))
        {
            auto & t = e.getComponent<CTransform>();
            size_t gridX = (size_t)(grid.width() * (t.p.x / world->width()));
            size_t gridY = (size_t)(grid.height() * (t.p.y / world->height()));
            double gridVal = grid.get(gridX, gridY);

            double diff = 0;
            if (gridVal < t1) { diff = std::abs(gridVal - t1); } 
            else if (gridVal > t2) { diff = std::abs(gridVal - t2); }
            sum += diff;
        }

        double maxDiff = std::max(t1, 1-t2);
        return 1 - ((sum / world->getEntities("puck").size()) / maxDiff);
    }
}