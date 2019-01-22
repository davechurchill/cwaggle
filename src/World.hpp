#pragma once

#include <vector>
#include <cassert>
#include <array>

#include "Vec2.hpp"
#include "CircleBody.hpp"
#include "Robot.hpp"
#include "Puck.hpp"
#include "Timer.hpp"
#include "LineBody.hpp"

class World
{
    // world properties
    double m_width              = 1920; // width  of the world 
    double m_height             = 1080; // height of the world 

    // stores all rigid circle bodies and collision data
    std::vector<CircleBody>     m_circles;
    std::vector<LineBody>       m_lines;
    
    // pucks and robots will eventually do things
    // they store pointers to their respective CircleBody stored in m_circles
    std::vector<Robot>          m_robots;
    std::vector<Puck>           m_pucks;

public:

    World(double width, double height)
        : m_width(width)
        , m_height(height)
    {
        m_circles.reserve(20000);
        m_robots.reserve(20000);
        m_pucks.reserve(20000);
    }

    void addRobot(const Vec2 & pos, double radius, std::array<uint8_t, 4> color = { 255, 0, 0, 255 })
    {
        // robots store pointers to their CircleBody object which live in m_circles
        // it's possible that these pointers become invalid once m_circles has to resize
        // so for now we'll just check to see if that's going to happen and assert
        assert(m_robots.empty() || m_robots.size() < m_robots.capacity());
        m_circles.emplace_back(CircleBody(pos, radius, m_circles.size()));
        m_robots.emplace_back(Robot(m_robots.size(), m_circles.back().id, color));
    }

    void addPuck(const Vec2 & pos, double radius, std::array<uint8_t, 4> color = { 0, 0, 255, 255 })
    {
        // pucks store pointers to their CircleBody object which live in m_circles
        // it's possible that these pointers become invalid once m_circles has to resize
        // so for now we'll just check to see if that's going to happen and assert
        assert(m_pucks.empty() || m_pucks.size() < m_pucks.capacity());
        m_circles.emplace_back(CircleBody(pos, radius, m_circles.size()));
        m_pucks.emplace_back(Puck(m_pucks.size(), m_circles.back().id, color));
    }

    void addLine(const Vec2 & start, const Vec2 & end, double radius)
    {
        m_lines.emplace_back(LineBody(start, end, radius));
    }

    std::vector<CircleBody> & getCircles()
    {
        return m_circles;
    }

    std::vector<Robot> & getRobots()
    {
        return m_robots;
    }

    std::vector<Puck> & getPucks()
    {
        return m_pucks;
    }

    std::vector<LineBody> & getLines()
    {
        return m_lines;
    }

    double width() const
    {
        return m_width;
    }

    double height() const
    {
        return m_height;
    }
};