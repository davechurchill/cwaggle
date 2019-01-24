#pragma once

#include <SFML/Graphics.hpp>
#include <sstream>

#include "Vec2.hpp"
#include "Simulator.hpp"
#include "ExampleWorlds.hpp"
#include "LineBody.hpp"
#include "SensorTools.hpp"

class GUI
{
    Simulator &         m_sim;
    sf::RenderWindow    m_window;           // the window we will draw to
    sf::Font            m_font;             // the font we will use to draw
    sf::Text            m_text;
    sf::Clock           m_clock;
    sf::Vector2f        m_mousePos;
    Entity              m_selected;
    Entity              m_shooting;
    LineBody *          m_selectedLine = nullptr;
    bool                m_selectedLineStart = false;
    bool                m_debug = false;
    bool                m_grid = false;
    bool                m_sensors = false;

    std::vector<sf::CircleShape> m_circleShapes;
    std::vector<sf::Color> m_colors;
    std::vector<sf::RectangleShape> m_gridRectangles;

    void init(WorldECS & world)
    {
        //m_sim.setWorld(world);
        m_font.loadFromFile("fonts/cour.ttf");
        m_text.setFont(m_font);
        m_text.setCharacterSize(24);
        m_text.setPosition(5, 5);
        m_text.setFillColor(sf::Color::Yellow);

        // reset the circle shapes vector
        m_circleShapes = std::vector<sf::CircleShape>(m_sim.getWorld().getEntities().size(), sf::CircleShape(1, 32));

        std::cout << world.getEntities().size() << "\n";
        std::cout << m_sim.getWorld().getEntities().size() << "\n";

        // add the robot circle shapes
        for (auto e : m_sim.getWorld().getEntities())
        {
            auto & b = e.getComponent<CBody>();
            auto & shape = m_circleShapes[e.id()];

            shape.setRadius((float)b.r);
            shape.setOrigin((float)b.r, (float)b.r);
            shape.setPosition((float)b.p.x, (float)b.p.y);
            shape.setOutlineThickness(0);

            auto & color = e.getComponent<CColor>();
            shape.setFillColor(sf::Color(color.r, color.g, color.b, color.a));
        }

        //// add the puck circle shapes
        //for (auto & puck : m_sim.getWorld().getPucks())
        //{
        //    auto & circle = m_sim.getWorld().getCircles()[puck.bodyID()];
        //    auto & shape = m_circleShapes[circle.id];

        //    shape.setRadius((float)circle.r);
        //    shape.setOrigin((float)circle.r, (float)circle.r);
        //    shape.setPosition((float)circle.p.x, (float)circle.p.y);
        //    shape.setOutlineThickness(0);

        //    auto color = puck.color();
        //    shape.setFillColor(sf::Color(color[0], color[1], color[2], color[3]));
        //}

        // create the grid rectangle shapes
        auto & grid = m_sim.getWorld().getGrid();
        if (grid.width() > 0)
        {
            double rWidth = m_sim.getWorld().width() / (double)grid.width();
            double rHeight = m_sim.getWorld().height() / (double)grid.height();
            sf::Vector2f rectSize((float)rWidth, (float)rHeight);

            // create the 2D grid of rectangles 
            m_gridRectangles = std::vector<sf::RectangleShape>(grid.width() * grid.height());

            // set the positions and colors of the rectangles
            for (size_t x = 0; x < grid.width(); x++)
            {
                for (size_t y = 0; y < grid.height(); y++)
                {
                    sf::Vector2f rPos(x * (float)rWidth, y * (float)rHeight);
                    uint8_t c = (uint8_t)(grid.get(x, y) * 255);
                    sf::Color color(c, c, c);
                    size_t index = y * grid.width() + x;
                    m_gridRectangles[index].setSize(rectSize);
                    m_gridRectangles[index].setFillColor(color);
                    m_gridRectangles[index].setPosition(rPos);
                }
            }
        }
    }

    void sUserInput()
    {
        sf::Event event;
        while (m_window.pollEvent(event))
        {
            // this event triggers when the window is closed
            if (event.type == sf::Event::Closed)
            {
                exit(0);
            }

            // this event is triggered when a key is pressed
            if (event.type == sf::Event::KeyPressed)
            {
                switch (event.key.code)
                {
                    case sf::Keyboard::Escape: exit(0); break;
                    /*case sf::Keyboard::Num1:   init(ExampleWorlds::GetGridWorld720(1)); break;
                    case sf::Keyboard::Num2:   init(ExampleWorlds::GetGridWorld720(2)); break;
                    case sf::Keyboard::Num3:   init(ExampleWorlds::GetGridWorld720(3)); break;
                    case sf::Keyboard::Num4:   init(ExampleWorlds::GetGridWorld720(4)); break;
                    case sf::Keyboard::Num5:   init(ExampleWorlds::GetGridWorld720(5)); break;
                    case sf::Keyboard::Num6:   init(ExampleWorlds::GetGridWorld720(6)); break;
                    case sf::Keyboard::Num7:   init(ExampleWorlds::GetGridWorld720(7)); break;
                    case sf::Keyboard::Num8:   init(ExampleWorlds::GetGridWorld720(8)); break;
                    case sf::Keyboard::Num9:   init(ExampleWorlds::GetGridWorld720(9)); break;*/
                    case sf::Keyboard::D:      m_debug = !m_debug; break;
                    case sf::Keyboard::G:      m_grid = !m_grid; break;
                    case sf::Keyboard::S:      m_sensors = !m_sensors; break;
                    default: break;
                }
            }

            if (event.type == sf::Event::MouseButtonPressed)
            {
                if (event.mouseButton.button == sf::Mouse::Left)
                {
                    for (auto e : m_sim.getWorld().getEntities())
                    {
                        Vec2 mPos((double)event.mouseButton.x, (double)event.mouseButton.y);
                        if (mPos.dist(e.getComponent<CBody>().p) < e.getComponent<CBody>().r)
                        {
                            m_selected = e;
                            break;
                        }
                    }

                    /*for (auto & line : m_sim.getWorld().getLines())
                    {
                        Vec2 mPos((double)event.mouseButton.x, (double)event.mouseButton.y);

                        if (mPos.dist(line.s) < line.r)
                        {
                            m_selectedLine = &line;
                            m_selectedLineStart = true;
                            break;
                        }

                        if (mPos.dist(line.e) < line.r)
                        {
                            m_selectedLine = &line;
                            m_selectedLineStart = false;
                            break;
                        }
                    }*/
                }

                if (event.mouseButton.button == sf::Mouse::Right)
                {
                    for (auto & e : m_sim.getWorld().getEntities())
                    {
                        Vec2 mPos((double)event.mouseButton.x, (double)event.mouseButton.y);
                        if (mPos.dist(e.getComponent<CBody>().p) < e.getComponent<CBody>().r)
                        {
                            m_shooting = e;
                            break;
                        }
                    }
                }
            }

            if (event.type == sf::Event::MouseButtonReleased)
            {
                if (event.mouseButton.button == sf::Mouse::Left)
                {
                    m_selected = Entity();
                    //m_selectedLine = Entity();
                }

                if (event.mouseButton.button == sf::Mouse::Right)
                {
                    if (m_shooting.id() != 0)
                    {
                        auto & t = m_shooting.getComponent<CBody>();
                        t.v.x = (t.p.x - m_mousePos.x) / 10.0f;
                        t.v.y = (t.p.y - m_mousePos.y) / 10.0f;
                        m_shooting = Entity();
                    }
                }
            }

            if (event.type == sf::Event::MouseMoved)
            {
                m_mousePos = sf::Vector2f((float)event.mouseMove.x, (float)event.mouseMove.y);
            }
        }


        if (m_selected.id() != 0)
        {
            auto & t = m_selected.getComponent<CBody>();
            Vec2 diff(m_mousePos.x - t.p.x, m_mousePos.y - t.p.y);
            diff /= 10;
            t.v = diff;
        }

        if (m_selectedLine != nullptr)
        {
            if (m_selectedLineStart)
            {
                m_selectedLine->s.x = m_mousePos.x;
                m_selectedLine->s.y = m_mousePos.y;
            }
            else
            {
                m_selectedLine->e.x = m_mousePos.x;
                m_selectedLine->e.y = m_mousePos.y;
            }
        }
    }

    void drawLine(Vec2 p1, Vec2 p2, sf::Color color)
    {
        sf::Vertex line[] =
        {
            sf::Vertex(sf::Vector2f((float)p1.x, (float)p1.y), color),
            sf::Vertex(sf::Vector2f((float)p2.x, (float)p2.y), color)
        };

        m_window.draw(line, 2, sf::Lines);
    }

    void sRender()
    {
        m_window.clear();


        // draw the value grid
        if (m_grid)
        {
            for (auto & rect : m_gridRectangles)
            {
                m_window.draw(rect);
            }
        }


        for (auto e : m_sim.getWorld().getEntities())
        {
            auto & b = e.getComponent<CBody>();

            m_circleShapes[e.id()].setPosition((float)b.p.x, (float)b.p.y);
            m_window.draw(m_circleShapes[e.id()]);

            Vec2 velPoint;
            double vLength = b.v.length();
            if (vLength == 0)
            {
                velPoint = Vec2(b.p.x + b.r, b.p.y);
                continue;
            }
            else
            {
                velPoint = b.p + b.v.normalize() * b.r;
            }

            drawLine(b.p, velPoint, sf::Color(255, 255, 255));
        }

        // draw robot sensors
        /*if (m_sensors)
        {
            float sensorRadius = 2;
            for (auto & robot : m_sim.getWorld().getRobots())
            {
                for (auto & sensor : robot.gridSensors())
                {
                    sf::CircleShape sensorShape(sensorRadius, 32);
                    sensorShape.setOrigin(sensorRadius, sensorRadius);
                    Vec2 pos = SensorTools::GetGridSensorPosition(sensor, m_sim.getWorld());
                    sensorShape.setPosition((float)pos.x, (float)pos.y);
                    sensorShape.setFillColor(sf::Color::White);
                    m_window.draw(sensorShape);
                }
            }
        }*/

        /*for (auto & line : m_sim.getWorld().getLines())
        {
            sf::CircleShape circle((float)line.r, 32);
            circle.setOrigin((float)line.r, (float)line.r);
            circle.setOutlineThickness(1);
            circle.setPosition((float)line.s.x, (float)line.s.y);
            m_window.draw(circle);
            circle.setPosition((float)line.e.x, (float)line.e.y);
            m_window.draw(circle);

            Vec2 normal(-(line.e.y - line.s.y), (line.e.x - line.s.x));
            normal = normal.normalize() * line.r;

            drawLine(line.s + normal, line.e + normal, sf::Color::White);
            drawLine(line.s - normal, line.e - normal, sf::Color::White);
        }*/

        if (m_debug)
        {
            for (auto & collision : m_sim.getCollisions())
            {
                drawLine(collision.b1->p, collision.b2->p, sf::Color::Green);
            }
        }

        if (m_shooting.id() != 0)
        {
            drawLine(m_shooting.getComponent<CBody>().p, Vec2(m_mousePos.x, m_mousePos.y), sf::Color::Red);
        }

        // draw score
        std::stringstream ss;
        ss << "Num Objs: " << m_sim.getWorld().getEntities().size() << "\n";
        ss << "CPU Time: " << m_sim.getComputeTime() << "ms\n";
        ss << "Max Time: " << m_sim.getComputeTimeMax() << "ms\n";
        ss << "Debug:    " << (m_debug ? "on" : "off");
        m_text.setString(ss.str());
        m_window.draw(m_text);

        m_window.display();
    }

    
public:

    GUI(Simulator & sim, size_t fps)
        : m_sim(sim)
    {
        m_window.create(sf::VideoMode((size_t)m_sim.getWorld().width(), (size_t)m_sim.getWorld().height()), "CWaggle");
        m_window.setFramerateLimit(fps);
        init(sim.getWorld());
    }

    void update()
    {
        sUserInput();
        sRender();
    }
};