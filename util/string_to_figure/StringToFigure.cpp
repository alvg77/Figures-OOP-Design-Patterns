#include "StringToFigure.hpp"

#include <algorithm>
#include <memory>
#include <sstream>
#include <vector>

#include "../../figure/circle/Circle.hpp"
#include "../../figure/rectangle/Rectangle.hpp"
#include "../../figure/triangle/Triangle.hpp"

std::unique_ptr<Figure> StringToFigure::create_figure(const std::string &representation) {
    std::stringstream sstream(representation);

    std::string figureName;
    std::vector<double> params;

    sstream >> figureName;
    std::ranges::transform(figureName, figureName.begin(),
                       [](const unsigned char c) { return std::tolower(c); });

    std::string temp;
    while (sstream >> temp) {
        params.push_back(std::stod(temp));
    }

    if (figureName == "triangle") {
        if (params.size() != 3) {
            throw std::invalid_argument("triangle requires three parameters");
        }
        return std::make_unique<Triangle>(params.at(0), params.at(1), params.at(2));
    }

    if (figureName == "circle") {
        if (params.size() != 1) {
            throw std::invalid_argument("circle requires one parameter");
        }
        return std::make_unique<Circle>(params.at(0));
    }

    if (figureName == "rectangle") {
        if (params.size() != 2) {
            throw std::invalid_argument("rectangle requires two parameters");
        }
        return std::make_unique<Rectangle>(params.at(0), params.at(1));
    }

    throw std::invalid_argument("Invalid figure string");
}

bool StringToFigure::is_double(const std::string &str) {
    try {
        double val = std::stod(str);
        return true;
    } catch (...) {
        return false;
    }
}
