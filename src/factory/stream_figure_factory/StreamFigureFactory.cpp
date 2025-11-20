#include "StreamFigureFactory.hpp"

#include <algorithm>
#include <iostream>
#include <stdexcept>
#include <sstream>

#include "../../util/figure_util/FigureUtil.hpp"
#include "../../util/string_to_figure/StringToFigure.hpp"

StreamFigureFactory::StreamFigureFactory(std::unique_ptr<std::istream> is) : is(std::move(is))
{
}

std::unique_ptr<Figure> StreamFigureFactory::create()
{
    std::istream &input = (is == nullptr) ? std::cin : *is;
    std::stringstream sstream;
    std::string figure;

    if (!(input >> figure))
    {
        return nullptr;
    }

    sstream << figure;

    std::ranges::transform(figure, figure.begin(), [](const unsigned char c) { return std::tolower(c); });

    const FigureUtil::FigureType type = FigureUtil::strToFigure(figure);

    const unsigned paramN = FigureUtil::getFigureParams(type);
    for (unsigned i = 0; i < paramN; i++)
    {
        std::string value;
        if (!(input >> value))
        {
            throw std::runtime_error("Cannot read from input stream!");
        }

        sstream << ' ';
        sstream << value;
    }

    return StringToFigure::createFigure(sstream.str());
}