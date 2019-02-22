#pragma once

#include "CWaggle.h"

struct OrbitalConstructionConfig
{
    double maxAngularSpeed = 0.3;
    double forwardSpeed = 2;
    double thresholds[2] = { 0.7, 0.8 };
};

namespace EntityControllers
{
    EntityAction OrbitalConstruction(Entity e, 
                                     std::shared_ptr<World> world, 
                                     const SensorReading & reading,
                                     const OrbitalConstructionConfig & config)
    {
        if (reading.leftObstacle > 0)
        {
            return EntityAction(config.forwardSpeed, config.maxAngularSpeed);
        }

        size_t type = e.getComponent<CRobotType>().type;
        bool innie = type == 1;

        if (reading.rightNest >= reading.midNest && reading.midNest >= reading.leftNest)
        {
            // The gradient is in the desired orientation with the highest
            // sensed value to the right, then the centre value in the middle,
            // followed by the lowest on the left.

            // These conditions steer in (for an innie) and out (for an outie)
            // to nudge a puck inwards or outwards.
            if (innie && reading.rightPucks > 0)
            {
                return EntityAction(config.forwardSpeed, config.maxAngularSpeed);
            }
            else if (!innie && reading.leftPucks > 0)
            {
                return EntityAction(config.forwardSpeed, -config.maxAngularSpeed);
            }

            // We now act to maintain the centre value at the desired isoline.
            if (reading.midNest < config.thresholds[type])
            {
                return EntityAction(config.forwardSpeed, 0.3 * config.maxAngularSpeed);
            }
            else
            {
                return EntityAction(config.forwardSpeed, -0.3 * config.maxAngularSpeed);
            }
        }
        else if (reading.midNest >= reading.rightNest && reading.midNest >= reading.leftNest)
        {
            // We are heading uphill of the gradient, turn left to return to a
            // clockwise orbit.
            return EntityAction(config.forwardSpeed, -config.maxAngularSpeed);
        }
        else
        {
            // We are heading downhill, turn right to return to clockwise orbit.
            return EntityAction(config.forwardSpeed, config.maxAngularSpeed);
        }
    }
};

