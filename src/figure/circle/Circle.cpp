#include "Circle.hpp"

#include <cmath>
#include <sstream>
#include <stdexcept>

void Circle::validateRadius(const double radius)
{
    if (radius <= 0 || !std::isfinite(radius))
    {
        throw std::invalid_argument("Radius must be a positive number");
    }
}

Circle::Circle(const double radius) : radius(radius)
{
    validateRadius(radius);

    if (!std::isfinite(Circle::perimeter()))
    {
        throw std::invalid_argument("Perimeter must be a positive number");
    }
}

double Circle::perimeter() const
{
    return 2 * M_PI * radius;
}

std::string Circle::to_string() const
{
    std::stringstream sstream;
    sstream << "Circle " << radius;
    return sstream.str();
}

Circle *Circle::clone() const
{
    return new Circle(radius);
}
