#ifndef FIGURES_TRIANGLE_HPP
#define FIGURES_TRIANGLE_HPP

#include "../Figure.hpp"

class Triangle final : public Figure
{
  private:
    const double a;
    const double b;
    const double c;

    static void validate_side(double side, const std::string &name);

    static void validate_triangle(double a, double b, double c);

  public:
    Triangle(double a, double b, double c);

    double perimeter() const override;

    std::string toString() const override;

    Triangle *clone() const override;
};

#endif // FIGURES_TRIANGLE_HPP
