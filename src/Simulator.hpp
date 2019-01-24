#pragma once

#include <vector>
#include <cassert>
#include <memory>
#include <algorithm>

#include "Vec2.hpp"
#include "CircleBody.hpp"
#include "Robot.hpp"
#include "Puck.hpp"
#include "Timer.hpp"
#include "World.hpp"

struct CollisionData
{
    CBody * b1;
    CBody * b2;
};

class Simulator
{
    WorldECS m_world;

    // physics configuration
    double m_timeStep           = 1.0;  // time step per update call
    double m_overlapThreshold   = 0.1;  // allow overlap of this amount without resolution
    double m_deceleration       = 0.2;  // deceleration multiplier, replace with friction
    double m_stoppingSpeed      = 0.001; // stop an object if moving less than this speed

    // time keeping
    double m_computeTime        = 0;    // the CPU time of the last frame of collisions
    double m_computeTimeMax     = 0;    // the max CPU time of collisions since init

    std::vector<CollisionData>  m_collisions;
    std::vector<CircleBody>     m_fakeBodies;

    void movement()
    {
        // update robot body's velocity from its heading and angle
        //for (auto & robot : m_world.getEntities())
        //{
        //    if (!robot.doAction()) { continue; }

        //    auto & body = m_world.getCircles()[robot.bodyID()];

        //    // update the body's velocity based on angle and speed
        //    body.v.x = robot.speed() * cos(robot.angle());
        //    body.v.y = robot.speed() * sin(robot.angle());
        //}

        // apply acceleration, velocity to all circles
        for (auto e : m_world.getEntities())
        {
            auto & b = e.getComponent<CBody>();

            if (b.v.length() < m_stoppingSpeed) { b.v = Vec2(0, 0); }
            b.a = b.v * -m_deceleration;
            b.p += b.v * m_timeStep; 
            b.v += b.a * m_timeStep;
            b.moved = fabs(b.v.x) > 0 || fabs(b.v.y) > 0;
        }
    }

    void collisions()
    {
        Timer timer;
        timer.start();
        m_collisions.clear();
        m_fakeBodies.clear();

        // we can skip collision checking for any circle that hasn't moved
        // static resolution doesn't alter speed, so movement not recorded
        // so if a circle collided last frame, consider it to have moved
        for (auto e : m_world.getEntities())
        {
            if (e.getComponent<CBody>().collided) { e.getComponent<CBody>().moved = true; }
            e.getComponent<CBody>().collided = false;
        }

        size_t numEntities = m_world.getEntities().size();
        auto & bodies = EntityMemoryPool::Instance().getBodies();

        size_t i1 = 0;

        // detect collisions for all circles
        //for (auto & e1 : m_world.getEntities())
        for (auto & b1 : bodies)
        {
            if (i1++ > numEntities) break;

            // step 1: check collisions of all circles against all lines
            //for (auto & edge : m_world.getLines())
            //{
            //    double lineX1 = edge.e.x - edge.s.x;
            //    double lineY1 = edge.e.y - edge.s.y;
            //    double lineX2 = c1.p.x - edge.s.x;
            //    double lineY2 = c1.p.y - edge.s.y;

            //    double edgeLength = lineX1 * lineX1 + lineY1 * lineY1;
            //    double dotProd = lineX1 * lineX2 + lineY1 * lineY2;
            //    double t = std::max(0.0, std::min(edgeLength, dotProd)) / edgeLength;

            //    // find the closest point on the line to the circle and the distance to it
            //    Vec2 closestPoint(edge.s.x + t * lineX1, edge.s.y + t * lineY1);
            //    double distance = closestPoint.dist(c1.p);

            //    // pretend the closest point on the line is a circle and check collision
            //    // calculate the overlap between the circle and that fake circle
            //    double overlap = c1.r + edge.r - distance;

            //    // if the circle and the line overlap
            //    if (overlap > m_overlapThreshold)
            //    {
            //        // create a fake circlebody to handle physics
            //        m_fakeBodies.emplace_back(CircleBody(closestPoint, edge.r, m_fakeBodies.size()));
            //        auto & body = m_fakeBodies.back();
            //        body.v = c1.v * -1.0;

            //        // add a collision between the circle and the fake circle
            //        // this will later be resolved in the dynamic collision resolution
            //        m_collisions.push_back({ &c1, &body });

            //        // resolve the static collision by pushing circle away from line
            //        // lines assume infinite mass and do not get moved
            //        c1.p.x += overlap * (c1.p.x - body.p.x) / distance;
            //        c1.p.y += overlap * (c1.p.y - body.p.y) / distance;
            //        c1.collided = true;
            //    }
            //}

            //auto & b1 = e1.getComponent<CBody>();

            // if this circle hasn't moved, we don't need to check collisions for it
            if (!b1.moved) { continue; }

            // step 2: check collisions of all circles against all other circles
            size_t i2 = 0;
            for (auto & b2 : bodies)
            {
                if (i2++ > numEntities) { break; }
                //auto & b2 = e2.getComponent<CBody>();

                if (b1.p.distSq(b2.p) > (b1.r + b2.r)*(b1.r + b2.r)) { continue; }
                double dist = b1.p.dist(b2.p);
                if (dist == 0) { continue; }

                // calculate the actual distance and overlap between circles
                //double dist = b1.p.dist(b2.p);
                double overlap = (b1.r + b2.r) - dist;

                // circles overlap if the overlap is positive
                if (overlap > m_overlapThreshold)
                {
                    // record that a collision took place between these two objects
                    m_collisions.push_back({ &b1, &b2 });

                    // calculate the static collision resolution (direct position modifier)
                    // scale how much we push each circle back in the static collision by mass ratio
                    Vec2 delta1 = (b1.p - b2.p) / dist * overlap * (b2.m / (b1.m + b2.m));
                    Vec2 delta2 = (b1.p - b2.p) / dist * overlap * (b1.m / (b1.m + b2.m));

                    // apply the static collision resolution and record collision
                    b1.p += delta1; b1.collided = true;
                    b2.p -= delta2; b2.collided = true;
                }
            }
            // wraparound behavior
            //if (c1.p.x < 0) { c1.p.x += m_world.width(); }
            //if (c1.p.y < 0) { c1.p.y += m_world.height(); }
            //if (c1.p.x >= m_world.width()) { c1.p.x -= m_world.width(); }
            //if (c1.p.y >= m_world.height()) { c1.p.y -= m_world.height(); }
            
            // check for collisions with the bounds of the world
            if (b1.p.x - b1.r < 0) { b1.p.x = b1.r; b1.collided = true; }
            if (b1.p.y - b1.r < 0) { b1.p.y = b1.r; b1.collided = true; }
            if (b1.p.x + b1.r > m_world.width()) { b1.p.x = m_world.width() - b1.r;  b1.collided = true; }
            if (b1.p.y + b1.r > m_world.height()) { b1.p.y = m_world.height() - b1.r; b1.collided = true; }
        }

        // step 3: calculate and apply dynamic collision resolution to any detected collisions
        for (auto & collision : m_collisions)
        {
            CBody & b1 = *collision.b1;
            CBody & b2 = *collision.b2;

            // normal between the circles
            double dist = b1.p.dist(b2.p);
            double nx = (b2.p.x - b1.p.x) / dist;
            double ny = (b2.p.y - b1.p.y) / dist;

            // thank you wikipedia
            // https://en.wikipedia.org/wiki/Elastic_collision
            double kx = (b1.v.x - b2.v.x);
            double ky = (b1.v.y - b2.v.y);
            double p = 2.0f * (nx*kx + ny * ky) / (b1.m + b2.m);
            b1.v.x -= p * b2.m * nx;
            b1.v.y -= p * b2.m * ny;
            b2.v.x += p * b1.m * nx;
            b2.v.y += p * b1.m * ny;
        }

        // record the time that this collision calculation took
        m_computeTime = timer.getElapsedTimeInMilliSec();
        m_computeTimeMax = m_computeTime > m_computeTimeMax ? m_computeTime : m_computeTimeMax;
    }

public:

    Simulator(const WorldECS & world)
        : m_world(world)
    {
        m_collisions.reserve(1000);
        m_fakeBodies.reserve(10000);
    }

    void update(double timeStep = 1.0)
    {
        m_timeStep = timeStep;
        movement();
        collisions();
    }

    void setWorld(const WorldECS & world)
    {
        //m_world = world;
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

    WorldECS & getWorld()
    {
        return m_world;
    }
};