#ifndef FIGURES_FIGURE_HPP
#define FIGURES_FIGURE_HPP

#include "../util/Clonable.hpp"
#include "../util/StringConvertible.hpp"

#include <ostream>

class Figure : public Clonable, public StringConvertible
{
  public:
    virtual double perimeter() const = 0;

    Figure *clone() const override = 0;

    ~Figure() override = default;

    friend std::ostream &operator<<(std::ostream &os, const Figure &figure);
};

std::ostream &operator<<(std::ostream &os, const Figure &figure);

#endif // FIGURES_FIGURE_HPP
