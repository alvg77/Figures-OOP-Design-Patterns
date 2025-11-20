#ifndef FIGURES_STRINGTOFIGURE_HPP
#define FIGURES_STRINGTOFIGURE_HPP

#include <string>

#include "../../figure/Figure.hpp"

class StringToFigure
{
  public:
    static std::unique_ptr<Figure> createFigure(const std::string &representation);
    static bool isDouble(const std::string &str);
};

#endif // FIGURES_STRINGTOFIGURE_HPP
