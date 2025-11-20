#include "StringToFigure.hpp"

#include <algorithm>
#include <memory>
#include <sstream>
#include <vector>

#include "../../figure/circle/Circle.hpp"
#include "../../figure/rectangle/Rectangle.hpp"
#include "../../figure/triangle/Triangle.hpp"

#include <iostream>

std::unique_ptr<Figure> StringToFigure::createFigure(const std::string &representation)
{
    std::stringstream sstream(representation);

    std::string figureName;
    std::vector<double> params;

    sstream >> figureName;
    std::ranges::transform(figureName, figureName.begin(), [](const unsigned char c) { return std::tolower(c); });

    std::string temp;
    while (sstream >> temp)
    {
        params.push_back(std::stod(temp));
    }

    if (figureName == "triangle")
    {
        if (params.size() != 3)
        {
            throw std::invalid_argument("Triangle requires three parameters");
        }
        return std::make_unique<Triangle>(params.at(0), params.at(1), params.at(2));
    }

    if (figureName == "circle")
    {
        if (params.size() != 1)
        {
            throw std::invalid_argument("Circle requires one parameter");
        }
        return std::make_unique<Circle>(params.at(0));
    }

    if (figureName == "rectangle")
    {
        if (params.size() != 2)
        {
            throw std::invalid_argument("Rectangle requires two parameters");
        }
        return std::make_unique<Rectangle>(params.at(0), params.at(1));
    }

    return nullptr;
}

bool StringToFigure::isDouble(const std::string &str)
{
    try
    {
        size_t pos;
        std::stod(str, &pos);
        return pos == str.size();
    }
    catch (...)
    {
        return false;
    }
}
