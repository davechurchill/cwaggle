#pragma once

#include <vector>
#include <cassert>
#include <memory>

#include "Vec2.hpp"
#include "CircleBody.hpp"
#include "Robot.hpp"
#include "Puck.hpp"
#include "Timer.hpp"
#include "World.hpp"

struct CollisionData
{
    CircleBody * c1 = nullptr;
    CircleBody * c2 = nullptr;
};

class Simulator
{
    World m_world;

    // physics configuration
    double m_overlapThreshold   = 0.1;  // allow overlap of this amount without resolution
    double m_deceleration       = 0.1;  // deceleration multiplier, replace with friction
    double m_stoppingSpeed      = 0.01; // stop an object if moving less than this speed

    // time keeping
    double m_computeTime        = 0;    // the CPU time of the last frame of collisions
    double m_computeTimeMax     = 0;    // the max CPU time of collisions since init

    std::vector<CollisionData>  m_collisions;

    void movement()
    {
        // update robot body's velocity from its heading and angle
        for (auto & robot : m_world.getRobots())
        {
            if (!robot.doAction()) { continue; }

            auto & body = m_world.getCircles()[robot.bodyID()];

            // update the body's velocity based on angle and speed
            body.v.x = robot.speed() * cos(robot.angle());
            body.v.y = robot.speed() * sin(robot.angle());
        }

        // apply acceleration, velocity to all circles
        for (auto & circle : m_world.getCircles())
        {
            if (circle.v.length() < m_stoppingSpeed) { circle.v = Vec2(0, 0); }
            circle.a = circle.v * -m_deceleration;
            circle.p += circle.v; 
            circle.v += circle.a;
            circle.moved = fabs(circle.v.x) > 0 || fabs(circle.v.y) > 0;
        }
    }

    void collisions()
    {
        Timer t;
        t.start();
        m_collisions.clear();

        // we can skip collision checking for any circle that hasn't moved
        // static resolution doesn't alter speed, so movement not recorded
        // so if a circle collided last frame, consider it to have moved
        for (auto & c1 : m_world.getCircles())
        {
            if (c1.collided) { c1.moved = true; }
            c1.collided = false;
        }

        // detect collisions for all circles with other circles
        for (auto & c1 : m_world.getCircles())
        {
            // if this circle hasn't moved, we don't need to check collisions for it
            if (!c1.moved) { continue; }

            // check against each other circle
            for (auto & c2 : m_world.getCircles())
            {
                if (c1.p.distSq(c2.p) > (c1.r + c2.r)*(c1.r + c2.r)) { continue; }
                if (c1.id == c2.id) { continue; }

                // calculate the actual distance and overlap between circles
                double dist = c1.p.dist(c2.p);
                double overlap = (c1.r + c2.r) - dist;

                // circles overlap if the overlap is positive
                if (overlap > m_overlapThreshold)
                {
                    // record that a collision took place between these two objects
                    m_collisions.push_back({ &c1, &c2 });

                    // calculate the static collision resolution (direct position modifier)
                    // scale how much we push each circle back in the static collision by mass ratio
                    Vec2 delta1 = (c1.p - c2.p) / dist * overlap * (c2.m / (c1.m + c2.m));
                    Vec2 delta2 = (c1.p - c2.p) / dist * overlap * (c1.m / (c1.m + c2.m));

                    // apply the static collision resolution and record collision
                    c1.p += delta1; c1.collided = true;
                    c2.p -= delta2; c2.collided = true;
                }
            }

            // check for collisions with the bounds of the world
            if (c1.p.x - c1.r < 0) { c1.p.x = c1.r;            c1.collided = true; }
            if (c1.p.y - c1.r < 0) { c1.p.y = c1.r;            c1.collided = true; }
            if (c1.p.x + c1.r > m_world.width()) { c1.p.x = m_world.width() - c1.r;  c1.collided = true; }
            if (c1.p.y + c1.r > m_world.height()) { c1.p.y = m_world.height() - c1.r; c1.collided = true; }
        }

        // calculate and apply dynamic collision resolution
        for (auto & collision : m_collisions)
        {
            auto * c1 = collision.c1;
            auto * c2 = collision.c2;

            // normal between the circles
            double dist = c1->p.dist(c2->p);
            double nx = (c2->p.x - c1->p.x) / dist;
            double ny = (c2->p.y - c1->p.y) / dist;

            // thank you wikipedia
            // https://en.wikipedia.org/wiki/Elastic_collision
            double kx = (c1->v.x - c2->v.x);
            double ky = (c1->v.y - c2->v.y);
            double p = 2.0f * (nx*kx + ny * ky) / (c1->m + c2->m);
            c1->v.x -= p * c2->m * nx;
            c1->v.y -= p * c2->m * ny;
            c2->v.x += p * c1->m * nx;
            c2->v.y += p * c1->m * ny;
        }

        // record the time that this collision calculation took
        m_computeTime = t.getElapsedTimeInMilliSec();
        m_computeTimeMax = m_computeTime > m_computeTimeMax ? m_computeTime : m_computeTimeMax;
    }

public:

    Simulator(const World & world)
        : m_world(world)
    {
        m_collisions.reserve(1000);
    }

    void update()
    {
        movement();
        collisions();
    }

    void setWorld(const World & world)
    {
        m_world = world;
    }

    std::vector<CollisionData> & getCollisions()
    {
        return m_collisions;
    }

    double getComputeTime() const
    {
        return m_computeTime;
    }

    double getComputeTimeMax() const
    {
        return m_computeTimeMax;
    }

    World & getWorld()
    {
        return m_world;
    }
};