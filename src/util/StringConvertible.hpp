#ifndef FIGURES_STRING_CONVERTIBLE_HPP
#define FIGURES_STRING_CONVERTIBLE_HPP

#include <string>

class StringConvertible
{
  public:
    virtual std::string to_string() const = 0;
    virtual ~StringConvertible() = default;
};

#endif // FIGURES_STRING_CONVERTIBLE_HPP