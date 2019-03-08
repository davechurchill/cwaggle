#pragma once

// For loading a grid from an image file.
#include <SFML/Graphics/Image.hpp>

#include "ValueGrid.hpp"
#include "Vec2.hpp"

namespace ExampleGrids
{
    // populates a world with a grid of pucks
    ValueGrid GetInverseCenterDistanceGrid(size_t width, size_t height)
    {
        ValueGrid grid(width, height);

        for (size_t x = 0; x < width; x++)
        {
            for (size_t y = 0; y < height; y++)
            {
                grid.set(x, y, Vec2(x, y).dist(Vec2(width / 2.0, height / 2.0)));
            }
        }

        grid.normalize();
        grid.invert();
        return grid;
    }

    ValueGrid GetGridFromFile(const std::string &filename)
    {
        sf::Image image;
        image.loadFromFile(filename);
        size_t width = image.getSize().x;
        size_t height = image.getSize().y;

        ValueGrid grid(width, height);

        for (size_t x = 0; x < width; x++)
        {
            for (size_t y = 0; y < height; y++)
            {
                grid.set(x, y, image.getPixel(x, y).r );
            }
        }

        grid.normalize();
        return grid;
    }

};