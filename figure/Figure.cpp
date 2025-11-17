#include "Figure.hpp"

std::ostream &operator<<(std::ostream &os, const Figure &figure) {
    os << figure.to_string() << '\n';
    return os;
}
