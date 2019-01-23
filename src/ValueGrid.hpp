#pragma once

#include <vector>
#include <algorithm>

class ValueGrid
{
    size_t m_width = 0;
    size_t m_height = 0;
    std::vector<double> m_values;
    
    inline size_t getIndex(size_t x, size_t y) const
    {
        return y * m_width + x;
    }

public:

    ValueGrid() {}
    ValueGrid(size_t width, size_t height, double value = 0.0)
        : m_width(width), m_height(height), m_values(width*height, value) { }

    inline double get(size_t x, size_t y) const
    {
        size_t index = getIndex(x, y);
        assert(index < m_values.size());
        return m_values[index];
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

    size_t width() const
    {
        return m_width;
    }

    size_t height() const
    {
        return m_height;
    }
};