#include "Triangle.hpp"

#include <cmath>
#include <iostream>
#include <sstream>
#include <stdexcept>

void Triangle::validate_side(const double side, const std::string &name)
{
    if (side <= 0 || !std::isfinite(side))
    {
        throw std::invalid_argument(name + "must be a positive finite value");
    }
}

void Triangle::validate_triangle(const double a, const double b, const double c)
{
    if (!((a + b > c) && (b + c > a) && (a + c > b)))
    {
        throw std::invalid_argument("No triangle with such sides exist!");
    }
}

Triangle::Triangle(const double a, const double b, const double c) : a(a), b(b), c(c)
{
    validate_side(a, "a");
    validate_side(b, "b");
    validate_side(c, "c");
    validate_triangle(a, b, c);

    if (!std::isfinite(Triangle::perimeter()))
    {
        throw std::invalid_argument("Cannot calculate perimeter");
    }
}

double Triangle::perimeter() const
{
    return a + b + c;
}

std::string Triangle::to_string() const
{
    std::stringstream sstream;
    sstream << "Triangle " << a << " " << b << " " << c;
    return sstream.str();
}

Triangle *Triangle::clone() const
{
    return new Triangle(a, b, c);
}
