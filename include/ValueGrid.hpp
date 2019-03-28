#pragma once

#include <vector>
#include <algorithm>
#include <assert.h>
#include <iostream>

#include <SFML/Graphics.hpp>

class ValueGrid
{
    size_t m_width = 0;
    size_t m_height = 0;
    std::vector<double> m_values;
    sf::Image m_image;

    inline size_t getIndex(size_t x, size_t y) const
    {
        return y * m_width + x;
    }


public:

    ValueGrid() {}
    ValueGrid(size_t width, size_t height, double value = 0.0)
        : m_width(width), m_height(height), m_values(width*height, value)
    {
        m_image.create(width, height, sf::Color::Black);
    }

    ValueGrid(const std::string & filename)
    {
        if (!m_image.loadFromFile(filename))
        {
            std::cerr << "ValueGrid file not found: " << filename << "\n";
            exit(-1);
        }

        m_width = m_image.getSize().x;
        m_height = m_image.getSize().y;

        m_values = std::vector<double>(m_width*m_height, 0);

        for (size_t x = 0; x < m_width; x++)
        {
            for (size_t y = 0; y < m_height; y++)
            {
                auto c = m_image.getPixel(x, y);
                set(x, y, ((c.r + c.g + c.b) / 3.0) / 255.0);
            }
        }
    }

    inline double get(size_t x, size_t y) const
    {
        if (x < 0 || y < 0 || x >= m_width || y >= m_height) return 0;
        size_t index = getIndex(x, y);

        if (index >= m_values.size()) return 0;
        assert(index < m_values.size());
        return m_values[index];
    }

    void setImage()
    {
        for (size_t x = 0; x < m_width; x++)
        {
            for (size_t y = 0; y < m_height; y++)
            {
                sf::Uint8 pixel = (sf::Uint8)(get(x, y) * 255);
                m_image.setPixel(x, y, sf::Color(pixel, pixel, pixel));
            }
        }
    }

    inline void set(size_t x, size_t y, double value)
    {
        size_t index = getIndex(x, y);
        assert(index < m_values.size());
        m_values[index] = value;
    }

    inline void normalize()
    {
        // max the min value a 0
        auto minVal = *std::min_element(std::begin(m_values), std::end(m_values));
        for (auto & val : m_values) { val -= minVal; }
        
        // divide everything by the max value
        auto maxVal = *std::max_element(std::begin(m_values), std::end(m_values));

        if (maxVal == 0) { return; }
        for (auto & val : m_values) {  val /= maxVal;  }
    }

    inline void invert()
    {
        for (auto & val : m_values) { val = 1.0 - val; }
    }

    const sf::Image & getImage() const
    {
        return m_image;
    }

    size_t width() const
    {
        return m_width;
    }

    size_t height() const
    {
        return m_height;
    }
};