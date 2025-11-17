#ifndef FIGURES_CIRCLE_HPP
#define FIGURES_CIRCLE_HPP

#include <string>

#include "../Figure.hpp"

class Circle final : public Figure {
private:
    const double radius;

    static void validateRadius(double radius);

public:
    explicit Circle(double radius);

    double perimeter() const override;

    std::string to_string() const override;

    Circle *clone() const override;
};

#endif //FIGURES_CIRCLE_HPP
