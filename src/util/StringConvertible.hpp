#ifndef FIGURES_STRING_CONVERTIBLE_HPP
#define FIGURES_STRING_CONVERTIBLE_HPP

#include <string>

class StringConvertible
{
  public:
    virtual std::string toString() const = 0;
    virtual ~StringConvertible() = default;
};

#endif // FIGURES_STRING_CONVERTIBLE_HPP