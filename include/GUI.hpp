#pragma once

#include <SFML/Graphics.hpp>
#include <sstream>
#include <iostream>

#include "Vec2.hpp"
#include "Simulator.hpp"
#include "ExampleWorlds.hpp"
#include "SensorTools.hpp"

class GUI
{
    std::shared_ptr<Simulator> m_sim;
    sf::RenderWindow    m_window;           // the window we will draw to
    sf::Font            m_font;             // the font we will use to draw
    sf::Text            m_text;
    sf::Clock           m_clock;
    sf::Vector2f        m_mousePos;
    sf::Texture         m_gridTexture;
    sf::Sprite          m_gridSprite;
    Entity              m_selected;
    Entity              m_shooting;
    Entity              m_selectedLine;
    bool                m_selectedLineStart = false;
    bool                m_debug = false;
    bool                m_grid = false;
    bool                m_sensors = false;
    std::string         m_status = "";

    void init(std::shared_ptr<Simulator> sim)
    {
        m_sim = sim;
        m_font.loadFromFile("fonts/cour.ttf");
        m_text.setFont(m_font);
        m_text.setCharacterSize(24);
        m_text.setPosition(5, 5);
        //m_text.setFillColor(sf::Color::Yellow);

        // create the grid rectangle shapes
        auto & grid = m_sim->getWorld()->getGrid();
        m_gridTexture.loadFromImage(grid.getImage());
        m_gridSprite = sf::Sprite(m_gridTexture);
        m_gridSprite.scale((float)m_window.getSize().x / grid.width(), (float)m_window.getSize().y / grid.height());
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
                    for (auto e : m_sim->getWorld()->getEntities())
                    {
                        Vec2 mPos((double)event.mouseButton.x, (double)event.mouseButton.y);
                        if (mPos.dist(e.getComponent<CTransform>().p) < e.getComponent<CCircleBody>().r)
                        {
                            m_selected = e;
                            break;
                        }
                    }

                    for (auto e: m_sim->getWorld()->getEntities("line"))
                    {
                        Vec2 mPos((double)event.mouseButton.x, (double)event.mouseButton.y);
                        auto & line = e.getComponent<CLineBody>();

                        if (mPos.dist(line.s) < line.r)
                        {
                            m_selectedLine = e;
                            m_selectedLineStart = true;
                            break;
                        }

                        if (mPos.dist(line.e) < line.r)
                        {
                            m_selectedLine = e;
                            m_selectedLineStart = false;
                            break;
                        }
                    }
                }

                if (event.mouseButton.button == sf::Mouse::Right)
                {
                    for (auto & e : m_sim->getWorld()->getEntities())
                    {
                        Vec2 mPos((double)event.mouseButton.x, (double)event.mouseButton.y);
                        if (mPos.dist(e.getComponent<CTransform>().p) < e.getComponent<CCircleBody>().r)
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
                    m_selectedLine = Entity();
                }

                if (event.mouseButton.button == sf::Mouse::Right)
                {
                    if (m_shooting != Entity())
                    {
                        auto & t = m_shooting.getComponent<CTransform>();
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


        if (m_selected != Entity())
        {
            auto & t = m_selected.getComponent<CTransform>();
            Vec2 diff(m_mousePos.x - t.p.x, m_mousePos.y - t.p.y);
            diff /= 10;
            t.v = diff;
        }

        if (m_selectedLine != Entity())
        {
            if (m_selectedLineStart)
            {
                m_selectedLine.getComponent<CLineBody>().s.x = m_mousePos.x;
                m_selectedLine.getComponent<CLineBody>().s.y = m_mousePos.y;
            }
            else
            {
                m_selectedLine.getComponent<CLineBody>().e.x = m_mousePos.x;
                m_selectedLine.getComponent<CLineBody>().e.y = m_mousePos.y;
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
            m_window.draw(m_gridSprite);
        }

        // draw circles
        for (auto e : m_sim->getWorld()->getEntities())
        {
            if (!e.hasComponent<CCircleShape>()) { continue; }

            auto & t = e.getComponent<CTransform>();
            auto & s = e.getComponent<CCircleShape>();
            auto & c = e.getComponent<CColor>();

            s.shape.setPosition((float)t.p.x, (float)t.p.y);
            s.shape.setFillColor(sf::Color(c.r, c.g, c.b));
            m_window.draw(s.shape);

            Vec2 velPoint;
            double vLength = t.v.length();
            if (vLength == 0)
            {
                velPoint = Vec2(t.p.x + s.shape.getRadius(), t.p.y);
                continue;
            }
            else
            {
                velPoint = t.p + t.v.normalize() * s.shape.getRadius();
            }

            drawLine(t.p, velPoint, sf::Color(255, 255, 255));
        }

        // draw robot sensors
        if (m_sensors)
        {
            float sensorRadius = 2;
            for (auto robot : m_sim->getWorld()->getEntities("robot"))
            {
                if (!robot.hasComponent<CSensorArray>()) { continue; }
                auto & sensors = robot.getComponent<CSensorArray>();
                auto & c = robot.getComponent<CColor>();

                for (auto & sensor : sensors.gridSensors)
                {
                    sf::CircleShape sensorShape(sensorRadius, 32);
                    sensorShape.setOrigin(sensorRadius, sensorRadius);
                    Vec2 pos = sensor->getPosition();
                    sensorShape.setPosition((float)pos.x, (float)pos.y);
                    sensorShape.setFillColor(sf::Color::White);
                    m_window.draw(sensorShape);
                }

                for (auto sensor : sensors.obstacleSensors)
                {
                    sf::CircleShape sensorShape((float)sensor->radius(), 32);
                    sensorShape.setOrigin((float)sensor->radius(), (float)sensor->radius());
                    Vec2 pos = sensor->getPosition();
                    sensorShape.setPosition((float)pos.x, (float)pos.y);
                    double reading = sensor->getReading(m_sim->getWorld());
                    if (reading > 0) { sensorShape.setFillColor(sf::Color(255, 255, 255, 80)); }
                    else { sensorShape.setFillColor(sf::Color(0, 255, 255, 80)); }
                    m_window.draw(sensorShape);
                }

                for (auto sensor : sensors.puckSensors)
                {
                    sf::CircleShape sensorShape((float)sensor->radius(), 32);
                    sensorShape.setOrigin((float)sensor->radius(), (float)sensor->radius());
                    Vec2 pos = sensor->getPosition();
                    sensorShape.setPosition((float)pos.x, (float)pos.y);
                    double reading = sensor->getReading(m_sim->getWorld());
                    if (reading > 0) { sensorShape.setFillColor(sf::Color(255, 255, 255, 80)); }
                    else { sensorShape.setFillColor(sf::Color(c.r, c.g, c.b, 80)); }
                    m_window.draw(sensorShape);
                }

            }
        }

        for (auto & e : m_sim->getWorld()->getEntities("line"))
        {
            auto & line = e.getComponent<CLineBody>();

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
        }

        if (m_debug)
        {
            for (auto & collision : m_sim->getCollisions())
            {
                drawLine(collision.t1->p, collision.t2->p, sf::Color::Green);
            }
        }

        if (m_selected != Entity() && m_selected.hasComponent<CSensorArray>())
        {
            auto & t = m_selected.getComponent<CTransform>();
            SensorReading reading;
            SensorTools::ReadSensorArray(m_selected, m_sim->getWorld(), reading);

            sf::Text text;
            text.setFont(m_font);
            text.setCharacterSize(12);
            text.setString(reading.toString());
            text.setPosition((float)t.p.x, (float)t.p.y);
            m_window.draw(text);
        }
        
        if (m_shooting != Entity())
        {
            drawLine(m_shooting.getComponent<CTransform>().p, Vec2(m_mousePos.x, m_mousePos.y), sf::Color::Red);
        }

        // draw information
        std::stringstream ss;
        ss << "Num Objs: " << m_sim->getWorld()->getEntities().size() << "\n";
        ss << "CPU Time: " << m_sim->getComputeTime() << "ms\n";
        ss << "Max Time: " << m_sim->getComputeTimeMax() << "ms\n";
        m_text.setString(ss.str());
        m_window.draw(m_text);

        // draw evaluation
        sf::Text text;
        text.setFont(m_font);
        text.setString(m_status);
        text.setCharacterSize(20);
        text.setPosition(5, (float)m_sim->getWorld()->height() - text.getLocalBounds().height-5);
        
        m_window.draw(text);

        m_window.display();
    }

    
public:

    GUI(std::shared_ptr<Simulator> sim, size_t fps)
        : m_sim(sim)
    {
        m_window.create(sf::VideoMode((size_t)m_sim->getWorld()->width(), (size_t)m_sim->getWorld()->height()), "CWaggle");
        m_window.setFramerateLimit(fps);
        init(sim);
    }

    void setStatus(const std::string & str)
    {
        m_status = str;
    }

    void setSim(std::shared_ptr<Simulator> sim)
    {
        init(sim);
    }

    void update()
    {
        sUserInput();
        sRender();
    }
};