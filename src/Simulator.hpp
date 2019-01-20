#pragma once

#include <vector>
#include <cassert>

#include "Vec2.hpp"
#include "CircleBody.hpp"
#include "Robot.hpp"
#include "Puck.hpp"
#include "Timer.hpp"

struct CollisionData
{
    CircleBody * c1 = nullptr;
    CircleBody * c2 = nullptr;
};

class Simulator
{
    // world properties
    double m_width              = 1920; // width  of the world 
    double m_height             = 1080; // height of the world 

    // physics configuration
    double m_overlapThreshold   = 0.1;  // allow overlap of this amount without resolution
    double m_deceleration       = 0.1;  // deceleration multiplier, replace with friction
    double m_stoppingSpeed      = 0.01; // stop an object if moving less than this speed

    // time keeping
    double m_computeTime = 0;    // the CPU time of the last frame of collisions
    double m_computeTimeMax = 0;    // the max CPU time of collisions since init

    // stores all rigid circle bodies and collision data
    std::vector<CircleBody> m_circles;
    std::vector<CollisionData> m_collisions;
    
    std::vector<Robot>      m_robots;
    std::vector<Puck>       m_pucks;

    void Simulator::movement()
    {
        // apply acceleration, velocity to all circles
        for (auto & circle : m_circles)
        {
            if (circle.v.length() < m_stoppingSpeed) { circle.v = Vec2(0, 0); }
            circle.a = circle.v * -m_deceleration;
            circle.v += circle.a;
            circle.p += circle.v;
            circle.moved = fabs(circle.v.x) > 0 || fabs(circle.v.y) > 0;
        }
    }

    void Simulator::collisions()
    {
        Timer t;
        t.start();
        m_collisions.clear();

        // we can skip collision checking for any circle that hasn't moved
        // static resolution doesn't alter speed, so movement not recorded
        // so if a circle collided last frame, consider it to have moved
        for (auto & c1 : m_circles)
        {
            if (c1.collided) { c1.moved = true; }
            c1.collided = false;
        }

        // detect collisions for all circles with other circles
        for (auto & c1 : m_circles)
        {
            // if this circle hasn't moved, we don't need to check collisions for it
            if (!c1.moved) { continue; }

            // check against each other circle
            for (auto & c2 : m_circles)
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
            if (c1.p.x + c1.r > m_width) { c1.p.x = m_width - c1.r;  c1.collided = true; }
            if (c1.p.y + c1.r > m_height) { c1.p.y = m_height - c1.r; c1.collided = true; }
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

    Simulator::Simulator(double width, double height)
        : m_width(width)
        , m_height(height)
    {
        m_robots.reserve(20000);
        m_pucks.reserve(20000);
        m_circles.reserve(20000);
        init(2);
    }

    void Simulator::init(size_t skip)
    {
        m_circles.clear();
        m_robots.clear();
        m_pucks.clear();
        m_collisions.clear();

        addRobot(Vec2(200, 360), 50);

        for (size_t i = 0; i < 140; i += skip)
        {
            for (size_t j = 0; j < 90; j += skip)
            {
                addPuck(Vec2(400.0f + (double)i * 10, 100.0f + (double)j * 10), skip*4.0f);
            }
        }

        m_computeTimeMax = 0;
    }

    void Simulator::addRobot(const Vec2 & pos, double radius)
    {
        assert(m_robots.empty() || m_robots.size() < m_robots.capacity());
        m_circles.emplace_back(CircleBody(pos, radius, m_circles.size()));
        m_robots.emplace_back(Robot(&m_circles.back(), m_robots.size()));
    }

    void Simulator::addPuck(const Vec2 & pos, double radius)
    {
        assert(m_pucks.empty() || m_pucks.size() < m_pucks.capacity());
        m_circles.emplace_back(CircleBody(pos, radius, m_circles.size()));
        m_pucks.emplace_back(Puck(&m_circles.back(), m_pucks.size()));
    }

    void Simulator::update()
    {
        movement();
        collisions();
    }

    std::vector<CircleBody> & Simulator::getCircles()
    {
        return m_circles;
    }

    std::vector<CollisionData> & Simulator::getCollisions()
    {
        return m_collisions;
    }

    double Simulator::getComputeTime() const
    {
        return m_computeTime;
    }

    double Simulator::getComputeTimeMax() const
    {
        return m_computeTimeMax;
    }

    std::vector<Robot> & Simulator::getRobots()
    {
        return m_robots;
    }

    std::vector<Puck> & Simulator::getPucks()
    {
        return m_pucks;
    }

    double Simulator::width() const
    {
        return m_width;
    }

    double Simulator::height() const
    {
        return m_height;
    }
};