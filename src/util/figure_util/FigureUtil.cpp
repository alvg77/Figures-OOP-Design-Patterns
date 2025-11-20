#include "FigureUtil.hpp"

#include <iostream>
#include <ostream>
#include <stdexcept>

FigureUtil::FigureType FigureUtil::strToFigure(const std::string &str)
{
    if (str == "triangle")
    {
        return TRIANGLE;
    }

    if (str == "circle")
    {
        return CIRCLE;
    }

    if (str == "rectangle")
    {
        return RECTANGLE;
    }

    throw std::invalid_argument("Invalid figure type: '" + str + "'");
}

unsigned FigureUtil::getFigureParams(const FigureType type)
{
    unsigned paramN = 0;

    switch (type)
    {
    case TRIANGLE:
        paramN = 3;
        break;
    case CIRCLE:
        paramN = 1;
        break;
    case RECTANGLE:
        paramN = 2;
        break;
    }

    return paramN;
}

FigureUtil::FigureType FigureUtil::getRandomFigureType(std::mt19937_64 &rng)
{
    std::uniform_int_distribution<unsigned> figureDist(0, FIGURE_NUM - 1);

    return static_cast<FigureType>(figureDist(rng));
}
