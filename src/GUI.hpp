#pragma once

#include <SFML/Graphics.hpp>
#include <sstream>

#include "Vec2.hpp"
#include "Simulator.hpp"
#include "ExampleWorlds.hpp"

class GUI
{
    Simulator &         m_sim;
    sf::RenderWindow    m_window;           // the window we will draw to
    sf::Font            m_font;             // the font we will use to draw
    sf::Text            m_text;
    sf::Clock           m_clock;
    sf::Vector2f        m_mousePos;
    CircleBody *        m_selected = nullptr;
    CircleBody *        m_shooting = nullptr;
    bool                m_debug = false;
    Vec2                m_speed;

    std::vector<sf::CircleShape> m_circleShapes;
    std::vector<sf::Color> m_colors;

    void init(const World & world)
    {
        m_sim.setWorld(world);
        m_circleShapes.clear();
        m_font.loadFromFile("fonts/cour.ttf");
        m_text.setFont(m_font);
        m_text.setCharacterSize(24);
        m_text.setPosition(5, 5);
        m_text.setFillColor(sf::Color::Yellow);

        for (auto & circle : m_sim.getWorld().getCircles())
        {
            sf::CircleShape shape((float)circle.r, 32);
            shape.setOrigin((float)circle.r, (float)circle.r);
            shape.setPosition((float)circle.p.x, (float)circle.p.y);
            shape.setOutlineThickness(0);
            shape.setFillColor(sf::Color(0, 100, 200));
            m_circleShapes.push_back(shape);
        }

        // set the color of the big circle
        auto & circle = m_sim.getWorld().getCircles().front();
        sf::CircleShape shape((float)circle.r, 32);
        shape.setOrigin((float)circle.r, (float)circle.r);
        shape.setPosition((float)circle.p.x, (float)circle.p.y);
        shape.setOutlineThickness(0);
        shape.setFillColor(sf::Color(200, 44, 44));
        shape.setOutlineColor(sf::Color(255, 255, 255, 127));
        m_circleShapes[0] = shape;
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
                case sf::Keyboard::Num1:   init(ExampleWorlds::GetGridWorld720(1)); break;
                case sf::Keyboard::Num2:   init(ExampleWorlds::GetGridWorld720(2)); break;
                case sf::Keyboard::Num3:   init(ExampleWorlds::GetGridWorld720(3)); break;
                case sf::Keyboard::Num4:   init(ExampleWorlds::GetGridWorld720(4)); break;
                case sf::Keyboard::Num5:   init(ExampleWorlds::GetGridWorld720(5)); break;
                case sf::Keyboard::Num6:   init(ExampleWorlds::GetGridWorld720(6)); break;
                case sf::Keyboard::Num7:   init(ExampleWorlds::GetGridWorld720(7)); break;
                case sf::Keyboard::Num8:   init(ExampleWorlds::GetGridWorld720(8)); break;
                case sf::Keyboard::Num9:   init(ExampleWorlds::GetGridWorld720(9)); break;
                case sf::Keyboard::D:      m_debug = !m_debug; break;
                }
            }

            if (event.type == sf::Event::MouseButtonPressed)
            {
                if (event.mouseButton.button == sf::Mouse::Left)
                {
                    for (auto & circle : m_sim.getWorld().getCircles())
                    {
                        Vec2 mPos((double)event.mouseButton.x, (double)event.mouseButton.y);
                        if (mPos.dist(circle.p) < circle.r)
                        {
                            m_selected = &circle;
                            break;
                        }
                    }
                }

                if (event.mouseButton.button == sf::Mouse::Right)
                {
                    for (auto & circle : m_sim.getWorld().getCircles())
                    {
                        Vec2 mPos((double)event.mouseButton.x, (double)event.mouseButton.y);
                        if (mPos.dist(circle.p) < circle.r)
                        {
                            m_shooting = &circle;
                            break;
                        }
                    }
                }
            }

            if (event.type == sf::Event::MouseButtonReleased)
            {
                if (event.mouseButton.button == sf::Mouse::Left)
                {
                    m_selected = nullptr;
                }

                if (event.mouseButton.button == sf::Mouse::Right)
                {
                    if (m_shooting != nullptr)
                    {
                        m_shooting->v.x = (m_shooting->p.x - m_mousePos.x) / 10.0f;
                        m_shooting->v.y = (m_shooting->p.y - m_mousePos.y) / 10.0f;
                        m_shooting = nullptr;
                    }
                }
            }

            if (event.type == sf::Event::MouseMoved)
            {
                m_mousePos = sf::Vector2f((float)event.mouseMove.x, (float)event.mouseMove.y);
            }
        }


        if (m_selected != nullptr)
        {
            Vec2 diff(m_mousePos.x - m_selected->p.x, m_mousePos.y - m_selected->p.y);
            diff /= 10;
            m_selected->v = diff;
        }
    }

    void sRender()
    {
        m_window.clear();

        for (auto & circle : m_sim.getWorld().getCircles())
        {
            m_circleShapes[circle.id].setPosition((float)circle.p.x, (float)circle.p.y);
            m_window.draw(m_circleShapes[circle.id]);

            Vec2 velPoint;
            double vLength = circle.v.length();
            if (vLength == 0)
            {
                velPoint = Vec2(circle.p.x + circle.r, circle.p.y);
                continue;
            }
            else
            {
                velPoint = circle.p + circle.v.normalize() * circle.r;
            }

            // draw velocity vectors
            sf::Vertex line[] =
            {
                sf::Vertex(sf::Vector2f((float)circle.p.x, (float)circle.p.y), sf::Color(160, 160, 160)),
                sf::Vertex(sf::Vector2f((float)velPoint.x, (float)velPoint.y), sf::Color(160, 160, 160))
            };

            m_window.draw(line, 2, sf::Lines);
        }

        for (auto & collision : m_sim.getCollisions())
        {
            sf::Vertex line[] =
            {
                sf::Vertex(sf::Vector2f((float)collision.c1->p.x, (float)collision.c1->p.y), sf::Color::Green),
                sf::Vertex(sf::Vector2f((float)collision.c2->p.x, (float)collision.c2->p.y), sf::Color::Green)
            };

            if (m_debug)
            {
                m_window.draw(line, 2, sf::Lines);
            }
        }

        if (m_shooting != nullptr)
        {
            sf::Vertex line[] =
            {
                sf::Vertex(sf::Vector2f((float)m_shooting->p.x, (float)m_shooting->p.y), sf::Color::Red),
                sf::Vertex(m_mousePos, sf::Color::Red)
            };

            m_window.draw(line, 2, sf::Lines);
        }

        // draw score
        std::stringstream ss;
        ss << "Num Objs: " << m_sim.getWorld().getCircles().size() << "\n";
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