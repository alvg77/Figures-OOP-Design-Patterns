#include "Rectangle.hpp"

#include <cmath>
#include <sstream>
#include <stdexcept>

void Rectangle::validateDimension(const double value, const std::string &name)
{
    if (value <= 0 || !std::isfinite(value))
    {
        throw std::invalid_argument(name + "must be a positive number");
    }
}

Rectangle::Rectangle(const double width, const double height) : width(width), height(height)
{
    validateDimension(width, "width");
    validateDimension(height, "height");

    if (!std::isfinite(Rectangle::perimeter()))
    {
        throw std::invalid_argument("perimeter must be a positive number");
    }
}

double Rectangle::perimeter() const
{
    return 2 * width + 2 * height;
}

std::string Rectangle::toString() const
{
    std::stringstream sstream;
    sstream << "Rectangle " << width << " " << height;
    return sstream.str();
}

Rectangle *Rectangle::clone() const
{
    return new Rectangle(width, height);
}
