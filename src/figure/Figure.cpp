#include "Figure.hpp"

std::ostream &operator<<(std::ostream &os, const Figure &figure)
{
    os << figure.toString() << '\n';
    return os;
}
