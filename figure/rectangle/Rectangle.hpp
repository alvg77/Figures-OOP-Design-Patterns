#ifndef FIGURES_RECTANGLE_HPP
#define FIGURES_RECTANGLE_HPP

#include <string>

#include "../Figure.hpp"

class Rectangle final : public Figure {
private:
    const double width;
    const double height;

    static void validateDimension(double value, const std::string &name);

public:
    Rectangle(double width, double height);

    double perimeter() const override;

    std::string to_string() const override;

    Rectangle *clone() const override;
};

#endif //FIGURES_RECTANGLE_HPP
